#ifndef POTENTIOMETER_HPP_
#define POTENTIOMETER_HPP_

#include <Arduino.h>

namespace UI {

class Potentionmeter {
   public:
    Potentionmeter(const char *name, uint32_t pot_pin)
        : potentiometer_pin(pot_pin) {
        pinMode(potentiometer_pin, INPUT);
        analogReadResolution(12);
        pot_name = name;
    }

    inline void spinSampler(uint32_t dt){
        //TODO: Add sampler and filter
    };


    float getReading(){return output_value;}

   private:
    inline uint32_t getSample() { return analogRead(potentiometer_pin); }
    uint32_t potentiometer_pin;
    const char *pot_name;
    float output_value;
};

}  // namespace UI

#endif