/*
 * pwm_stepper.c
 *
 *  Created on: Apr 22, 2020
 *      Author: alsai
 */
#include "Actuators/pwm_stepper.h"
#include "main.h"
#include "math.h"

int32_t x_current; // Motor position in steps (1.8deg per step)
int32_t x_next;

extern TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigOCtim4 = { 0 };



static MotorParameter_t p;
static bool running = false;

static void update_sqw_frequency(uint32_t period_us) {
	/* Attempt a single pulse mode, at the end of each pulse its reconfigured and shot?
	 * but is this efficient?
	 * TODO: test single pulse then continuous and update on change only */
	HAL_GPIO_DeInit(M1PWM_GPIO_Port, M1PWM_Pin);
	bool idle = false;
	if (period_us == 0) {
		idle = true;
		period_us = 1E3;
		running = false;
	}

	static uint32_t timer_rate = 1E5; //Hz TODO: make function of clock setting
	uint32_t period = (uint64_t) period_us * timer_rate / 1E6;
	htim4.Init.Period = period;
	sConfigOCtim4.Pulse = period / 2;
	if (HAL_TIM_Base_Init(&htim4) == HAL_OK
			&& HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOCtim4,
			TIM_CHANNEL_4) == HAL_OK) {
		HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_4);
	} else {
		Error_Handler();
	}

	if (!idle) {
		HAL_TIM_MspPostInit(&htim4);
	}

	/* Note: To disengage motor, disable the M1EN pin (set High)
	 *		HAL_GPIO_WritePin(M1EN_GPIO_Port, M1EN_Pin, GPIO_PIN_SET);
	 * */
}

static int32_t motor_control_pulse_update_direct() {
	static int32_t v = 200; // steps / s
	static int32_t x_togo;

	x_togo = x_next - x_current;

	int dirSign = x_togo > 0 ? 1 : -1;
	int v_next = 0;
	if (x_togo == 0) {
		v_next = 0;
	} else {
		v_next = dirSign * v;
	}

	if (v_next * dirSign > 0) {
		x_current += dirSign;
	}

	uint32_t next_Tus =
			(v_next == 0) ? 0 : (1E6 / (uint32_t) (dirSign * v_next)); //
	update_sqw_frequency(next_Tus);

}

typedef enum {
	IDLE = 0, ACCELERATE, CRUISE, DECELERATE
} MOTION_STATE_T;

static int32_t motor_control_pulse_update_smooth() {

	static int32_t v_current = 0;
	static int32_t Tus = -1;
	static int32_t x_togo;
	static MOTION_STATE_T motion_state;

	if (Tus == -1) {
		Tus = 1E6 / p.v_min_start;
	}

	x_togo = x_next - x_current;

	int dirSign = x_togo > 0 ? 1 : -1;

	/* Change direction on low speed */

	if (x_togo != 0 && v_current == 0){
		if (dirSign > 0){
			HAL_GPIO_WritePin(M1DIR_GPIO_Port, M1DIR_Pin, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(M1DIR_GPIO_Port, M1DIR_Pin, GPIO_PIN_RESET);
		}
	}

	if (x_togo == 0) {
		motion_state = IDLE;
	} else if (v_current * dirSign < p.v_cruise
			&& x_togo * dirSign * 2 * p.acc > (v_current * v_current - p.v_min_start * p.v_min_start)) {
		motion_state = ACCELERATE;
	} else if (v_current * dirSign >= p.v_cruise
			|| x_togo * dirSign * 2 * p.acc >= (v_current * v_current  - p.v_min_start * p.v_min_start) ) {
		motion_state = CRUISE;
	} else if (x_togo * dirSign * 2 * p.acc < (v_current * v_current - p.v_min_stop * p.v_min_stop)) {
		motion_state = DECELERATE;
	} else {
		/* Should not reach here */
		int a = 1;
	}

	int32_t v_next = 0; // steps / s

	if (motion_state == IDLE) {
		v_next = 0;

	} else if (motion_state == CRUISE) {
		v_next = dirSign * p.v_cruise;
	} else if (motion_state == ACCELERATE) {
		v_next = v_current + p.acc * dirSign * Tus / 1E6;
		if (v_next * dirSign < p.v_min_start) {
			v_next = p.v_min_start * dirSign;
		} else if (v_next * dirSign > p.v_cruise) {
			v_next = dirSign * p.v_cruise;
		}
	} else if (motion_state == DECELERATE) {
		v_next = v_current - p.acc * dirSign * Tus / 1E6;
		if (v_next * dirSign < p.v_min_stop) {
			v_next = dirSign * p.v_min_stop;
		}
	}

	if (motion_state != IDLE) {
		x_current += dirSign;
	}

	uint32_t period=0;

	if(v_next == 0){
		Tus = 1E6 / p.v_min_start;
	}else{
		period = (1E6 / (uint32_t) (dirSign * v_next));
		Tus = period;
	}

	update_sqw_frequency(period);

	v_current = v_next;

}

static int32_t motor_control_pulse_update() {
	//TODO: change to millistep / s units
	static int32_t v_current; // steps / s
	static int32_t Tus = -1;
	static int32_t x_togo;

	/* Apply velocity trapezoid based on a_max, v_cruise, and steps to go. Remember: v^2 - v0^2 = 2a(x - x0) for constant a */

	if (Tus == -1) {
		Tus = 1E6 / p.v_min_start;
	}
	x_togo = x_next - x_current;

	int dirSign = x_togo > 0 ? 1 : -1;

	int32_t v_next = 0; // steps / s
	if (x_togo == 0) {
		v_next = 0;
	} else {
		if ((int32_t) (x_togo * dirSign * (2 * p.acc))
				<= (int32_t) (v_current * v_current
						- p.v_min_stop * p.v_min_stop)) {
			/* Decelerate */
			v_next = v_current - dirSign * p.acc * Tus / 1E6;

			if (v_next * dirSign < p.v_min_stop) {
				v_next = 0;
			}
			if (v_next * dirSign > p.v_cruise) {
				v_next = dirSign * p.v_cruise;
			}
		} else if (v_current * dirSign < p.v_cruise) {
			/* Accelerate */
			v_next = v_current + dirSign * p.acc * Tus / 1E6;
			/* Minimum speed */
			if (v_next * dirSign < p.v_min_start) {
				v_next = dirSign * p.v_min_start;
			}
			if (v_next * dirSign > p.v_cruise) {
				v_next = dirSign * p.v_cruise;
			}
		} else if (v_current * dirSign >= p.v_cruise) {
			/* Cruise */
			v_next = p.v_cruise * dirSign;
		} else {
			/* Should not reach this point*/
			return -1;
		}
	}

	/* Adjust PWM frequency TODO: how to extend without restarting or generating multiple pulses. */
	uint32_t next_Tus =
			(v_next == 0) ? 0 : (1E6 / (uint32_t) (dirSign * v_next)); //
	update_sqw_frequency(next_Tus);

	//TOOD: check if rate is valid then update Tus
	Tus = next_Tus; // this is the pulse period in microseconds

	v_current = v_next;
	if (v_next * dirSign > 0) {
		x_current += dirSign;
	}

	return 1;
}

void goto_position(int32_t steps) {
	x_next = steps;
	if (!running) {
		running = true;
		motor_control_pulse_update_switch();
	}
}

void update_motor_parameters(MotorParameter_t par) {
	p = par;

}

void move_steps(int32_t steps) {
	HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_2);
	x_next += steps;
	if (!running) {
		running = true;
		motor_control_pulse_update_smooth();
	}
}

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
//	if (htim->Instance == TIM4) {
//		motor_control_pulse_update_smooth();
//	}
//}
