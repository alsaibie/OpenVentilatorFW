/*
 * PSensInterface.hpp
 *
 *  Created on: May 1, 2020
 *      Author: alsai
 */

#ifndef SENSORS_PSENSINTERFACE_HPP_
#define SENSORS_PSENSINTERFACE_HPP_

#include "Device/I2C.hpp"

typedef struct {

  uint32_t filter_cutoff_frequency { };
  uint32_t sensitivity{};
  int32_t bias{};

  //TODO: Ensure the i2c buffer size captures the size of the struct
} PressureSensorsParameters_t;

class PressureSensorInterface {
 public:
  PressureSensorInterface(I2CMaster &i2c_interface, uint16_t device_address,
                          PressureSensorsParameters_t p) :
      i2ci(i2c_interface),
      i2c_address(device_address),
      params(p) {

  }

  enum class WriteCommands {
    All = 0x00,
    Filter_CutOff,
    Sampling_Freq,
    Calibrate
  //TODO: Add more adv filtering commands
  };

  enum class ReadCommands {
    All = 0x30,
    Pressure_mmH2O,
  //TODO: what else?
  };

  inline void spinSampler(uint32_t dt) {
    if (!started) {
      sendParameters();
      started = true;
    }

    /* Poll sample */
    i2ci.read(i2c_address, static_cast<uint8_t>(ReadCommands::Pressure_mmH2O),
              static_cast<uint8_t *>(&input_value), sizeof(input_value));

    /* Filter ? */

    output_value = static_cast<float>(input_value);

  }

  inline float getSample() {
    return output_value;
  }

  inline void setParameters(const PressureSensorsParameters_t p) {
    params = p;
    sendParameters();
  }

  inline void sendParameters() {
    i2ci.write(i2c_address, static_cast<uint8_t>(WriteCommands::All),
               static_cast<uint8_t*>(&params), sizeof(params));
  }

  inline void calibrate(){
    // TODO: implement interactive calibration routine
    i2ci.write(i2c_address, static_cast<uint8_t>(WriteCommands::Calibrate), NULL, 0);
  }

 private:
  PressureSensorsParameters_t params { };
  uint16_t i2c_address { 0x00 };
  float output_value { 0 };
  float input_value { 0 };
  bool started { false };
  I2CMaster &i2ci;

};

#endif /* SENSORS_PSENSINTERFACE_HPP_ */
