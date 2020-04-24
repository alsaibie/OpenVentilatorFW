/*
 * pwm_stepper.h
 *
 *  Created on: Apr 22, 2020
 *      Author: alsai
 */

#ifndef ACTUATORS_PWM_STEPPER_H_
#define ACTUATORS_PWM_STEPPER_H_

#include "stm32f4xx_hal.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int32_t acc; // steps/s^2
	int32_t v_cruise; // steps/s
	int32_t v_min_start; // step/s
	int32_t v_min_stop; // step/s can theoretically go to a lower speed with initial w
	uint32_t steps_per_rev;
} MotorParameter_t;

void update_motor_parameters(MotorParameter_t par);

void move_steps(int32_t steps);

#ifdef __cplusplus
}
#endif



#endif /* ACTUATORS_PWM_STEPPER_H_ */
