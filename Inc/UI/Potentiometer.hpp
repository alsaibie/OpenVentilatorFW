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
      static int deadband = 10;
      next_input = getSample();
      if (abs(next_input-previous_input) <  deadband){
        return;
      }
      output_value = 0.632 * previous_input + 0.368 * previous_output;
    	previous_input = next_input;
    	previous_output = output_value;
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
