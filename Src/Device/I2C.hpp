/*
 * I2C.hpp
 *
 *  Created on: May 1, 2020
 *      Author: alsai
 */

#ifndef DEVICE_I2C_HPP_
#define DEVICE_I2C_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


class I2CMaster {
 public:
  I2CMaster(I2C_HandleTypeDef &hi2c);

  HAL_StatusTypeDef write(uint16_t device_address, uint8_t write_cmd_header,
             uint8_t *write_buf, size_t len);

  HAL_StatusTypeDef read(uint16_t device_address, uint8_t read_cmd_header, uint8_t *read_buf,
            size_t len);

  void setAddress(uint16_t address);

  bool transmitting { false };

  bool receiving { false };

 private:

  void resetI2C();

  void WaitOrReset();


  static const uint16_t MAXRXBUFFERSIZE { 32 };
  uint8_t aRxBuffer[MAXRXBUFFERSIZE] { };
  static const uint16_t MAXTXBUFFERSIZE { 32 };
  uint8_t aTxBuffer[MAXTXBUFFERSIZE] { };

  I2C_HandleTypeDef handle;
};

I2CMaster * i2c1ptr_master;

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_I2C_HPP_ */
