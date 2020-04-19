/*
 * interrupts.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */
#include "Device/exti_interrupts.hpp"
#include "stm32f4xx_hal.h"

#define NUM_OF_EXTI   16

typedef struct {
  IRQn_Type irqn;
  callback_t cb;
} exti_cb_list_t;



/* Adopted from stm32duino
 * Only 16 pins can be used as EXTI pins, they can't have the same number: e.g. can't use both PB0 and PC0 together */

static exti_cb_list_t exti_cb_list[NUM_OF_EXTI] = {
    { .irqn = EXTI0_IRQn, .cb = NULL },  //GPIO_PIN_0
    { .irqn = EXTI1_IRQn, .cb = NULL },  //GPIO_PIN_1
    { .irqn = EXTI2_IRQn, .cb = NULL },  //GPIO_PIN_2
    { .irqn = EXTI3_IRQn, .cb = NULL },  //GPIO_PIN_3
    { .irqn = EXTI4_IRQn, .cb = NULL },  //GPIO_PIN_4
    { .irqn = EXTI9_5_IRQn, .cb = NULL },  //GPIO_PIN_5
    { .irqn = EXTI9_5_IRQn, .cb = NULL },  //GPIO_PIN_6
    { .irqn = EXTI9_5_IRQn, .cb = NULL },  //GPIO_PIN_7
    { .irqn = EXTI9_5_IRQn, .cb = NULL },  //GPIO_PIN_8
    { .irqn = EXTI9_5_IRQn, .cb = NULL },  //GPIO_PIN_9
    { .irqn = EXTI15_10_IRQn, .cb = NULL },  //GPIO_PIN_10
    { .irqn = EXTI15_10_IRQn, .cb = NULL },  //GPIO_PIN_11
    { .irqn = EXTI15_10_IRQn, .cb = NULL },  //GPIO_PIN_12
    { .irqn = EXTI15_10_IRQn, .cb = NULL },  //GPIO_PIN_13
    { .irqn = EXTI15_10_IRQn, .cb = NULL },  //GPIO_PIN_14
    { .irqn = EXTI15_10_IRQn, .cb = NULL }  //GPIO_PIN_15
};

uint32_t get_pin_exti_id(uint32_t GPIO_Pin) {
  uint32_t extid = 0;

  while (GPIO_Pin != 0x0001) {
    GPIO_Pin = GPIO_Pin >> 1;
    extid++;
  }
  return extid;
}

void attach_exti_to_cb(uint32_t GPIO_Pin, callback_t cb_) {
  uint8_t extid = get_pin_exti_id(GPIO_Pin);
  uint8_t get_pin_exti_id(GPIO_Pin);
  exti_cb_list[extid].cb = cb_;

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

  uint8_t extid = get_pin_exti_id(GPIO_Pin);
  if (exti_cb_list[extid].cb != NULL) {
    exti_cb_list[extid].cb();
  }

}

