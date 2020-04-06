#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <Arduino.h>

namespace UI {

class Button {
   public:
    Button(const char *name, uint32_t b_pin, callback_function_t cb_p) : button_pin(b_pin) {
        button_name = name;
        pinMode(button_pin, INPUT_PULLDOWN);
        attachInterrupt(button_pin, cb_p, FALLING);
        // TODO: change this so it accepts arguments. Can an ISR accept arguments? probably not
    }

   private:
    uint32_t button_pin;
    const char *button_name;
};

}  // namespace UI

#endif