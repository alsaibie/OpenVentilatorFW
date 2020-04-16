/*
 * interrupts.hpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */

#ifndef INTERRUPTS_HPP_
#define INTERRUPTS_HPP_

#ifdef __cplusplus

extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal_def.h"

#ifdef __cplusplus
}

#include <functional>
typedef std::function<void(void)> callback_t;
void attach_exti_to_cb(uint32_t GPIO_Pin, callback_t cb);
#endif

#endif /* INTERRUPTS_HPP_ */
