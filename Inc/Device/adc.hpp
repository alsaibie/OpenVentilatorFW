/*
 * adc.hpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */

#ifndef ADC_HPP_
#define ADC_HPP_


#ifdef __cplusplus

extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_adc.h"

#define NUM_OF_ADC_PINS 4
#define NUM_ADC1_CHANNNELS 4

typedef struct {
  ADC_TypeDef * adc_peri;
  GPIO_TypeDef * gpio_port;
  uint32_t gpio_pin;
  uint32_t channel_idx;
  uint32_t order;
}ADC_Pin_t;


/* Hardcode the ADC pins combo - how many will there be.. */
static ADC_Pin_t POT1_ADCPin = {ADC1, POT1_GPIO_Port, POT1_Pin, ADC_CHANNEL_0, 0};
static ADC_Pin_t POT2_ADCPin = {ADC1, POT2_GPIO_Port, POT2_Pin, ADC_CHANNEL_3, 1};
static ADC_Pin_t POT3_ADCPin = {ADC1, POT3_GPIO_Port, POT3_Pin, ADC_CHANNEL_9, 2};
static ADC_Pin_t PSENS_ADCPin = {ADC1, PSENS_GPIO_Port, PSENS_Pin, ADC_CHANNEL_10, 3};
static ADC_Pin_t FSENS_ADCPin = {ADC1, FSENS_GPIO_Port, FSENS_Pin, ADC_CHANNEL_13, 4};

uint16_t get_adc_reading(ADC_Pin_t &adc_pin);

#ifdef __cplusplus
}

#endif


#endif /* ADC_HPP_ */
