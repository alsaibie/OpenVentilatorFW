/*
 * Stepper.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: alsai
 */

#include "Actuators/Stepper.hpp"
#include "main.h"


using namespace Actuators;

void Stepper::moveSteps(int32_t steps) {
	HAL_TIM_PWM_Stop_IT(&pwm_timer, pwm_channel);

	step_next += steps;

	if (!running) {
		running = true;
		pulseUpdate();
	}
}


void Stepper::moveToStepPosition(int32_t step_pos){
	step_next = step_pos;
	if (!running) {
		running = true;
		pulseUpdate();
	}
}

void Stepper::moveWithVelocity(int32_t){

}

int32_t Stepper::pulseUpdate(){

	if (Tus == -1) {
		Tus = 1E6 / param.v_min_start;
	}

	steps_togo = step_next - step_postion;

	int dirSign = steps_togo > 0 ? 1 : -1;

	/* Change direction on low speed */

	if (steps_togo != 0 && velocity_est == 0){
		if (dirSign > 0){
			HAL_GPIO_WritePin(pins.dir.GPIOx, pins.dir.pin, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(pins.dir.GPIOx, pins.dir.pin, GPIO_PIN_RESET);
		}
	}

	if (steps_togo == 0) {
		motion_state = MotionState_t::IDLE;
	} else if (velocity_est * dirSign < param.v_cruise
			&& steps_togo * dirSign * 2 * param.acc > (velocity_est * velocity_est - param.v_min_start * param.v_min_start)) {
		motion_state = MotionState_t::ACCELERATE;
	} else if (velocity_est * dirSign >= param.v_cruise
			|| steps_togo * dirSign * 2 * param.acc >= (velocity_est * velocity_est  - param.v_min_start * param.v_min_start) ) {
		motion_state = MotionState_t::CRUISE;
	} else if (steps_togo * dirSign * 2 * param.acc < (velocity_est * velocity_est - param.v_min_stop * param.v_min_stop)) {
		motion_state = MotionState_t::DECELERATE;
	} else {
		/* Should not reach here */
		int a = 1;
	}

	int32_t v_next = 0; // steps / s

	if (motion_state == MotionState_t::IDLE) {
		v_next = 0;

	} else if (motion_state == MotionState_t::CRUISE) {
		v_next = dirSign * param.v_cruise;
	} else if (motion_state == MotionState_t::ACCELERATE) {
		v_next = velocity_est + param.acc * dirSign * Tus / 1E6;
		if (v_next * dirSign < param.v_min_start) {
			v_next = param.v_min_start * dirSign;
		} else if (v_next * dirSign > param.v_cruise) {
			v_next = dirSign * param.v_cruise;
		}
	} else if (motion_state == MotionState_t::DECELERATE) {
		v_next = velocity_est - param.acc * dirSign * Tus / 1E6;
		if (v_next * dirSign < param.v_min_stop) {
			v_next = dirSign * param.v_min_stop;
		}
	}

	if (motion_state != MotionState_t::IDLE) {
		step_postion += dirSign;
	}

	uint32_t period=0;

	if(v_next == 0){
		Tus = 1E6 / param.v_min_start;
	}else{
		period = (1E6 / (uint32_t) (dirSign * v_next));
		Tus = period;
	}

	updateSqwFrequency(period);

	velocity_est = v_next;

}


int32_t Stepper::pulseUpdateSimple(){

	steps_togo = step_next - step_postion;

	int32_t dirSign = steps_togo > 0 ? 1 : -1;
	int32_t v_next = 0;
	if (steps_togo == 0) {
		v_next = 0;
	} else {
		v_next = dirSign * param.v_cruise;
	}

	if (v_next * dirSign > 0) {
		step_postion += dirSign;
	}

	uint32_t next_Tus =
			(v_next == 0) ? 0 : (1E6 / (uint32_t) (dirSign * v_next)); //
	updateSqwFrequency(next_Tus);

}

void Stepper::updateSqwFrequency(uint32_t period_us){
	/* Attempt a single pulse mode, at the end of each pulse its reconfigured and shot?
	 * but is this efficient?
	 * TODO: test single pulse then continuous and update on change only */
	HAL_GPIO_DeInit(pins.pwm.GPIOx, pins.pwm.pin);
//	HAL_GPIO_DeInit(M1PWM_GPIO_Port, M1PWM_Pin);
	bool idle = false;
	if (period_us == 0) {
		idle = true;
		period_us = 1E3;
		running = false;
	}

	uint32_t period = (uint64_t) period_us * timer_rate / 1E6;
	pwm_timer.Init.Period = period;
	pwm_tim_ch_cfg.Pulse = period / 2;
	if (HAL_TIM_Base_Init(&pwm_timer) == HAL_OK
			&& HAL_TIM_PWM_ConfigChannel(&pwm_timer, &pwm_tim_ch_cfg,
			pwm_channel) == HAL_OK) {
		HAL_TIM_PWM_Start_IT(&pwm_timer, pwm_channel);
	} else {
//		Error_Handler();
	}

	if (!idle) {
		HAL_TIM_MspPostInit(&pwm_timer);
	}

	/* Note: To disengage motor, disable the M1EN pin (set High)
	 *		HAL_GPIO_WritePin(M1EN_GPIO_Port, M1EN_Pin, GPIO_PIN_SET);
	 * */
}


