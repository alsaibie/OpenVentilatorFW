#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "stm32f4xx_hal.h"
#include "Device/exti_interrupts.hpp"

namespace UI {

class Button {
   public:
    Button(const char *name, uint32_t b_pin, callback_t cb_p) : button_pin(b_pin) {
        button_name = name;
//        pinMode(button_pin, INPUT_PULLDOWN);
        attach_exti_to_cb(button_pin, cb_p);
        // TODO: change this so it accepts arguments. Can an ISR accept arguments? probably not
    }

   private:
    uint32_t button_pin;
    const char *button_name;
};

}  // namespace UI

#endif
