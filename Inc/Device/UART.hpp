/*
 * UART.hpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */

#ifndef DEVICE_UART_HPP_
#define DEVICE_UART_HPP_

#include "stm32f4xx_hal.h"
#include <string>


class UART {
 public:
  UART(UART_HandleTypeDef &handle)
      :
      huart(handle) {
  }

  void start();

  /* Synchronous call to check for new char in dma buffer  */
  void spin_circular();

  /* Print Methods */
  ssize_t print(const char *buf, size_t len);

  ssize_t print(const char *buf);

  ssize_t print(const std::string &s);

  size_t write(uint8_t &c);

  size_t write(const uint8_t *buf, size_t len);

  size_t write(const char *buf, size_t len);

  /* Define what to do when a new line is received */
  virtual void process_line(const uint8_t *buf, size_t len) = 0;

  inline UART_HandleTypeDef get_uart_handle(){return huart;}

 protected:

  ssize_t tx_dma_buffer(const uint8_t *buf, size_t len);

  ssize_t rx_dma_buffer(const uint8_t *buf, size_t len);


 private:

  static const uint_least8_t UART_RX_BUFFER_SIZE = 128;
  static const uint_least8_t UART_TX_BUFFER_SIZE = 128;

  uint8_t dma_rx_buffer[UART_RX_BUFFER_SIZE];

  uint8_t dma_tx_buffer[UART_TX_BUFFER_SIZE];

  uint32_t dma_receive_prev_idx;

  UART_HandleTypeDef &huart;

};




#endif /* DEVICE_UART_HPP_ */
