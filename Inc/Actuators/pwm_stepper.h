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
void update_motor_parameters();
void move_steps(int32_t steps);

#ifdef __cplusplus
}
#endif



#endif /* ACTUATORS_PWM_STEPPER_H_ */
