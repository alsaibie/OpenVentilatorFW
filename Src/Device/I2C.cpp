/*
 * I2C.cpp
 *
 *  Created on: May 1, 2020
 *      Author: alsai
 */

#include "I2C.hpp"
#include "main.h"
#include <cstring>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define I2C_TIMEOUT_TICKS 1000

SemaphoreHandle_t xI2CTxSemaphore = NULL;
SemaphoreHandle_t xI2CRxSemaphore = NULL;

I2CMaster::I2CMaster(I2C_HandleTypeDef &hi2c) :
    handle(hi2c) {

  xI2CTxSemaphore = xSemaphoreCreateBinary();
  xI2CRxSemaphore = xSemaphoreCreateBinary();

}

HAL_StatusTypeDef I2CMaster::write(uint16_t device_address,
                                   uint8_t write_cmd_header, uint8_t *write_buf,
                                   size_t len) {

  uint16_t leftalg_addres = device_address << 1;
  HAL_StatusTypeDef status = HAL_ERROR;

  memset(aTxBuffer, 0, MAXTXBUFFERSIZE);
  aTxBuffer[0] = write_cmd_header;
  memcpy((aTxBuffer + 1), write_buf, len);

  WaitOrReset();

  do {
    status = HAL_I2C_Master_Transmit_IT(&handle, leftalg_addres,
                                        (uint8_t*) aTxBuffer, 1);
    if (status != HAL_OK) {
      resetI2C();
      vTaskDelay(1);
    }

  } while (status != HAL_OK);  //TODO: Maybe change to error != HAL_I2C_ERROR_AF ?

  if (xSemaphoreTake( xI2CTxSemaphore, 1000 ) == pdTRUE) {

    WaitOrReset();

    do {
      status = HAL_I2C_Master_Transmit_IT(&handle, leftalg_addres,
                                          (uint8_t*) (aTxBuffer + 1), len);
      if (status != HAL_OK) {
        resetI2C();  //TODO: do with a timeout?
        vTaskDelay(1);
      }

    } while (status != HAL_OK);

    WaitOrReset();

  } else {
    return HAL_TIMEOUT;
  }

  return status;

}

HAL_StatusTypeDef I2CMaster::read(uint16_t device_address,
                                  uint8_t read_cmd_header, uint8_t *read_buf,
                                  size_t len) {

  uint16_t leftalg_addres = device_address << 1;
  HAL_StatusTypeDef status = HAL_ERROR;

  memset(aTxBuffer, 0, MAXTXBUFFERSIZE);
  aTxBuffer[0] = read_cmd_header;

  WaitOrReset();

  do {
    status = HAL_I2C_Master_Transmit_IT(&handle, leftalg_addres,
                                        (uint8_t*) aTxBuffer, 1);
    if (status != HAL_OK) {
      resetI2C();
      vTaskDelay(1);
    }

  } while (status != HAL_OK);

  if (xSemaphoreTake( xI2CTxSemaphore, 1000 ) == pdTRUE) {
    WaitOrReset();

    memset(aRxBuffer, 0, MAXRXBUFFERSIZE);

    HAL_I2C_Master_Receive_IT(&handle, leftalg_addres, (uint8_t*) aRxBuffer,
                              len + 1);

    if (xSemaphoreTake( xI2CRxSemaphore, 1000 ) == pdTRUE) {
      /* Verify Header, TODO: or else? */
      if (aRxBuffer[0] == read_cmd_header) {
        memcpy(read_buf, (aRxBuffer + 1), len);
        //TODO: also assert proper size ?
      }
    } else {
      return HAL_TIMEOUT;
    }
  } else {
    return HAL_TIMEOUT;
  }

  return status;

}

void I2CMaster::setAddress(uint16_t address) {
  /* Set own slave address */
  handle.Init.OwnAddress1 = (address << 1);

  if (HAL_I2C_Init(&handle) != HAL_OK) {
    Error_Handler();
  }
}

void I2CMaster::resetI2C() {

  HAL_I2C_DeInit(&handle);
  HAL_I2C_Init(&handle);

}

void I2CMaster::WaitOrReset() {

  uint32_t last_tick = HAL_GetTick();

  HAL_I2C_StateTypeDef i2c_state = HAL_I2C_STATE_ERROR;

  i2c_state = HAL_I2C_GetState(&handle);

  while (i2c_state != HAL_I2C_STATE_READY) {
    i2c_state = HAL_I2C_GetState(&handle);

    vTaskDelay(1);

    if (HAL_GetTick() - last_tick > I2C_TIMEOUT_TICKS) {
      resetI2C();
    }
  }
}

/* Registered I2C Instances */

uint32_t count_tx = 0;
uint32_t count_rx = 0;

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle) {

  if (I2cHandle->Instance == I2C1) {
    static BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xI2CTxSemaphore, &xHigherPriorityTaskWoken);
    count_tx++;
  }

}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle) {

  if (I2cHandle->Instance == I2C1) {
    static BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xI2CRxSemaphore, &xHigherPriorityTaskWoken);
    count_rx++;
  }

}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle) {

  if (I2cHandle->Instance == I2C1) {

  }

}
