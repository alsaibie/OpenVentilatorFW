#ifndef SWITCH_HPP_
#define SWITCH_HPP_

#include <Arduino.h>

namespace Sensors {

typedef enum { NONINVERTED = 0, INVERTED } SWITCH_LOGIC_T;

class Switch {
   public:
    Switch(const char *name, uint32_t sw_pin, callback_function_t cb_p, SWITCH_LOGIC_T lg)
        : switch_pin(sw_pin), switch_logic(lg) {
        switch_name = name;
        pinMode(switch_pin, INPUT_PULLDOWN);
        if (switch_logic == NONINVERTED) {
            attachInterrupt(switch_pin, cb_p, FALLING);
        } else if (switch_logic == INVERTED) {
            attachInterrupt(switch_pin, cb_p, RISING);
        }

        // TODO: change this so it accepts arguments. Can an ISR accept arguments? probably not
    }

   private:
    uint32_t switch_pin;
    const char *switch_name;
    SWITCH_LOGIC_T switch_logic;
};

}  // namespace Sensors

#endif