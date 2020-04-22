/*
 * adc.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */

#include "Device/adc.hpp"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
static const float Vref = 3.3f;

uint32_t adc1_values[NUM_OF_ADC_PINS] = {0, 0};
extern ADC_HandleTypeDef hadc1;

uint16_t get_adc_reading(ADC_Pin_t &adc_pin){

  /* Start DMA If Not Started */

  static bool dma_started = false;

  if (!dma_started){
    HAL_ADC_Start_DMA(&hadc1, adc1_values, NUM_ADC1_CHANNNELS);
    vTaskDelay(5);
    dma_started = true;
  }

  if(adc_pin.adc_peri == ADC1){
    #ifdef NUM_ADC1_CHANNNELS
        return adc1_values[adc_pin.order];
    #else
        return 0;
    #endif
  }else if (adc_pin.adc_peri == ADC2){
    #ifdef NUM_ADC2_CHANNNELS
        return adc2_values[adc_pin.order];
    #else
        return 0;
    #endif
  } else if (adc_pin.adc_peri == ADC3){
  #ifdef NUM_ADC3_CHANNNELS
        return adc3_values[adc_pin.order];
  #else
        return 0;
  #endif
  } else{
    return 0;
  }

}


