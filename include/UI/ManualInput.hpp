#include <Arduino.h>

#include "board_configuration.hpp"
/* Interface to pass callback functions to changes in input states */

namespace UI {
typedef enum { detectLOW = 0, detectHIGH, detectCHANGE, detectFALLING, detectRISING } EXT_INT_detectMODE_T;

inline void set_pin_input_callback(uint32_t pin, callback_function_t cb_, EXT_INT_detectMODE_T mode) {
    attachInterrupt(pin, cb_, mode);
}
}  // namespace UI

