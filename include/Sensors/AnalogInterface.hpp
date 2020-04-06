#ifndef ANALOGINTERFACE_HPP_
#define ANALOGINTERFACE_HPP_

#include <Arduino.h>

namespace Sensors {
class AnalogInterface {
    virtual void spinSampler(uint32_t dt) = 0;

   protected:
    inline uint32_t getSample() { return analogRead(analog_pin); }
    AnalogInterface(const char *name, uint32_t a_p) : analog_pin(a_p) {
        pinMode(analog_pin, INPUT);
        analogReadResolution(12);
        interface_name = name;
    }

   private:
    uint32_t analog_pin;
    const char *interface_name;
};
}  // namespace Sensors

#endif