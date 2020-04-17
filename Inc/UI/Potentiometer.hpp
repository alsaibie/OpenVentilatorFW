#ifndef POTENTIOMETER_HPP_
#define POTENTIOMETER_HPP_

#include "stm32f4xx_hal.h"
#include "Device/adc.hpp"
namespace UI {

class Potentionmeter {
   public:
    Potentionmeter(const char *name, ADC_Pin_t p_pin)
        : potentiometer_pin(p_pin), output_value(0), next_input(0), previous_output(0), previous_input(0) {

        pot_name = name;
    }

    inline void spinSampler(uint32_t dt){
        //TODO: Add sampler and filter


    	previous_input = next_input;
    	next_input = getSample() / 1.32;
    	previous_output = output_value;
      	output_value =  (next_input + previous_input) + (-0.51 * previous_output) ;

    };


    float getReading(){return output_value;}

   private:
    inline uint32_t getSample() { return get_adc_reading(potentiometer_pin); }
    ADC_Pin_t potentiometer_pin;
    const char *pot_name;
    float output_value;
	float next_input;
	float previous_output;
	float previous_input;
};

}  // namespace UI

#endif
