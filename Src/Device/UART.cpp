/*
 * UART.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: Ali AlSaibie
 *      Email:  ali.a@ku.edu.kw
 */


#include "Device/UART.hpp"
#include <cstring>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "main.h"
#define LF '\n'
#define CR '\r'
#define CRLF '\r\n'

static bool mutex = false;

void UART::start() {
  /* This starts a circular non-blocking receive task -
   * only called once, or again if paused/reset/faulted */
  HAL_UART_Receive_DMA(&huart, dma_rx_buffer, sizeof(dma_rx_buffer));
  dma_receive_prev_idx = UART_RX_BUFFER_SIZE - huart.hdmarx->Instance->NDTR;
}


void UART::spin_circular() {
  if (huart.ErrorCode != HAL_UART_ERROR_NONE) {

    HAL_UART_AbortReceive(&huart);
    HAL_UART_Receive_DMA(&huart, dma_rx_buffer, sizeof(dma_rx_buffer));

  };
//  while (HAL_UART_GetState(&huart) != HAL_UART_STATE_READY)
//  {}

  /* Grab the dma buffer idx where last character was place */
  uint32_t dma_receive_idx = UART_RX_BUFFER_SIZE - huart.hdmarx->Instance->NDTR;

  /* Circulate over the ring buffer from the last idx checked to the new idx
   * this routine should be called at a faster rate than Baud / Buffer Size  */
  if (dma_receive_idx != dma_receive_prev_idx) {
    if (dma_receive_idx > dma_receive_prev_idx) {
      /* underflow */
      rx_dma_buffer(&dma_rx_buffer[dma_receive_prev_idx], dma_receive_idx - dma_receive_prev_idx);
    } else if (dma_receive_idx < dma_receive_prev_idx) {
      /* overflow */
      rx_dma_buffer(&dma_rx_buffer[dma_receive_prev_idx], UART_RX_BUFFER_SIZE - dma_receive_prev_idx);
      /* Continue processing flowed */
      if (dma_receive_idx > 0) {
        rx_dma_buffer(&dma_rx_buffer[0], dma_receive_idx);
      }
    }
  }

  dma_receive_prev_idx = dma_receive_idx;
  if (dma_receive_idx == UART_RX_BUFFER_SIZE) {
    dma_receive_prev_idx = 0;
  }
}

ssize_t UART::rx_dma_buffer(const uint8_t *buf, size_t len) {
  if (len < 1) {
    return 0;
  }

  /* Read a line then parse message */
  static size_t idx = 0;
  const size_t line_buffer_size = 255; /* Hold complete message lines */
  static char line_buffer[line_buffer_size] = { };

  size_t k = 0;

  while (len--) {
    char c = (char) buf[k];

    if (c < 0 || c == LF || c == CR || c == CRLF) { /* Accept any line termination */
      process_line((uint8_t*) line_buffer, strlen(line_buffer));
      memset(line_buffer, 0, line_buffer_size);
      idx = 0;
    }

    line_buffer[idx] = c;
    idx++;
    k++;
  }

//TODO: return size read

//TODO: manage long lines error (if no \n is sent via uart)

}


size_t UART::write(uint8_t &c){
	return tx_dma_buffer(&c, 1);
}
size_t UART::write(const uint8_t *buf, size_t len){
	 return tx_dma_buffer(buf, len);
}

size_t UART::write(const char *buf, size_t len){
	return tx_dma_buffer((uint8_t*) buf, len);
}


ssize_t UART::print(const char *buf, size_t len) {
  return tx_dma_buffer((uint8_t*) buf, len);
}


size_t UART::print(const char *buf) {
  size_t len = 0;
  while(*(buf++) != 0){
    len++;
  }
  return tx_dma_buffer((uint8_t*)buf, len);

}

size_t UART::EOL(){
	const uint8_t a = 10;
	return tx_dma_buffer(&a, 1);
}

ssize_t UART::print(const std::string &s) {
  return tx_dma_buffer((uint8_t*) s.c_str(), s.length());
}

ssize_t UART::tx_dma_buffer(const uint8_t *buf, size_t len) {



	/* Loop if tx length is > tx buffer size */
	size_t llen = len;
	while (len) {
		size_t packet_len =
				len < UART_TX_BUFFER_SIZE ? len : UART_TX_BUFFER_SIZE;
		memcpy(dma_tx_buffer, buf, packet_len);

		while ((huart.gState != HAL_UART_STATE_READY));

		if (HAL_UART_Transmit_DMA(&huart, dma_tx_buffer, packet_len) != HAL_OK)
			return -1;
		buf += packet_len;
		len -= packet_len;
	}

	return llen;
}



/* UART Callbacks  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  /* Not useful as far as I can tell since the process is on a circular buffer */

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

}


