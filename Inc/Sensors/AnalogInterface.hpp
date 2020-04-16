#ifndef ANALOGINTERFACE_HPP_
#define ANALOGINTERFACE_HPP_

#include "stm32f4xx_hal.h""
#include "Device/adc.hpp"

namespace Sensors {
class AnalogInterface {
  virtual void spinSampler(uint32_t dt) = 0;

 protected:
  inline uint32_t getSample() {
      return get_adc_reading(adc_pin);
  }
  AnalogInterface(const char *name, ADC_Pin_t &a_p)
      :
        adc_pin(a_p) {
    interface_name = name;
  }

 private:
  ADC_Pin_t &adc_pin;
  const char *interface_name;
};
}  // namespace Sensors

#endif
