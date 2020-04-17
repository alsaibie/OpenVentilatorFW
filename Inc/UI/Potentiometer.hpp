#ifndef POTENTIOMETER_HPP_
#define POTENTIOMETER_HPP_

#include "stm32f4xx_hal.h"
#include "Device/adc.hpp"
namespace UI {

class Potentionmeter {
   public:
    Potentionmeter(const char *name, ADC_Pin_t p_pin)
        : potentiometer_pin(p_pin) {

        pot_name = name;
    }

    inline void spinSampler(uint32_t dt){
        //TODO: Add sampler and filter
    	output_value = getSample();
    };


    float getReading(){return output_value;}

   private:
    inline uint32_t getSample() { return get_adc_reading(potentiometer_pin); }
    ADC_Pin_t potentiometer_pin;
    const char *pot_name;
    float output_value;
};

}  // namespace UI

#endif
