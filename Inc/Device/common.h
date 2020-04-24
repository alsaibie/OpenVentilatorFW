/*
 * common.h
 *
 *  Created on: Apr 22, 2020
 *      Author: alsai
 */

#ifndef DEVICE_COMMON_H_
#define DEVICE_COMMON_H_

typedef struct {
	GPIO_TypeDef *GPIOx;
	uint16_t pin;
}GPIOPinCombo_t;

#endif /* DEVICE_COMMON_H_ */
