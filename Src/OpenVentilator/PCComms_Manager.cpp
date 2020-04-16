#include "OpenVentilator/OpenVentilator.hpp"

#include "Device/UART.hpp"

#include "OVTopics/operation_status.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input_offboard.hpp"

#include "semphr.h"

#include <cstring>

#include "ArduinoJson.h"

//#define DEBUG_PRINT

using namespace OVRTOS;
using namespace OVTopics;
SemaphoreHandle_t xDebugPrintSemaphore = NULL;

typedef struct _STREAM {
  char *buf;
  size_t len;
} STREAM;

#define UART_DEBUG_TX_STREAM_LENGTH 15
STREAM UART_DEBUG_TX_STREAM[UART_DEBUG_TX_STREAM_LENGTH];

int16_t uart_debug_tx_stream_idx = -1;

#define clear_stream(stream) {(stream).buf = NULL; (stream).len = 0;}

#define vcom_huart huart3
extern UART_HandleTypeDef vcom_huart;

#define ECHO_TEST false

class VCUART : public UART {
 public:
  VCUART(STREAM &inc_stream)
      :
      UART(vcom_huart),
      incoming_str(inc_stream) {

  }

  virtual void process_line(const uint8_t *buf, size_t len) {

#if ECHO_TEST
     tx_dma_buffer(buf, len);
     tx_dma_buffer((uint8_t*)"\n", 1);
  #endif

    memcpy(incoming_str.buf, buf, len);
    incoming_str.len = len;

  }

 private:
  STREAM &incoming_str;

};

class PCCommsManager : public OVThread {
 public:
  PCCommsManager()
      :
      OVThread("PC Communications", 1024, pc_comms_m_priority, 100),
      VComUART(incoming_rx_stream),
      user_input_offboard_pub(gUserInputOffboardOVQHandle),
      operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek),
      sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
      system_status_sub(gSystemStatusOVQHandle, &on_system_status_peek, Peek) {
  }

 protected:
  virtual void run() final {
    VComUART.start();

    const size_t CAPACITY = JSON_ARRAY_SIZE(3);
    StaticJsonDocument<CAPACITY> doc;

    while (1) {
      VComUART.spin_circular();
      operation_status_sub.receive();
      sensor_status_sub.receive();
      system_status_sub.receive();

      /* Send States to PC */
                  jdoc["Test"] = "GPS";
                  jdoc["Time"] = HAL_GetTick();
                  jdoc["Data1"] = 3.23;
                  jdoc["Data2"] = 3.230;
//                  JsonArray data = jdoc.createNestedArray("data");
//                  data.add(3);
//                  data.add(23);
                  JsonArray array = doc.to<JsonArray>();
                  array.add("hello");
                  array.add(42);
                  array.add(3.14);
                  serializeJson(doc, VComUART);

                  VComUART.print("\n");

      /* Receive Commands from PC */

      /* Relay to OVTopic */

      UserInputOffboard_msg_t ui_ob_msg;
      //
      user_input_offboard_pub.publish(ui_ob_msg);
      //
      /* Forward debug tx messages TOOD: remove after embedding debug into JSON */
      /* Check incoming */
#ifdef DEBUG_PRINT
      tx_debug_messages();
#endif
      debug_print("PCComm Manager Thread\n");

      if (incoming_rx_stream.len != 0) {
        /* Process new line TODO: make a stream array to handle a backlog */

        clear_stream(incoming_rx_stream);
      }

      thread_lap();
    }
  }

 private:

  void tx_debug_messages() {
    if (xSemaphoreTake(xDebugPrintSemaphore, (TickType_t) 10) == pdTRUE) {
      while (uart_debug_tx_stream_idx >= 0) {
        if (uart_debug_tx_stream_idx == 3) {
          HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        }

        VComUART.print(UART_DEBUG_TX_STREAM[uart_debug_tx_stream_idx].buf);

        clear_stream(UART_DEBUG_TX_STREAM[uart_debug_tx_stream_idx]);

        uart_debug_tx_stream_idx--;

      }

      xSemaphoreGive(xDebugPrintSemaphore);
    } else {
      /* For debugging purposes */
    }
  }
  STREAM incoming_rx_stream;
  VCUART VComUART;

  static void on_operation_status_peek(const OperationStatus_msg_t &msg) {
  }
  static void on_sensor_status_peek(const SensorStatus_msg_t &msg) {
  }
  static void on_system_status_peek(const SystemStatus_msg_t &msg) {
  }
  /* Pubs */
  OVQueuePublisher<UserInputOffboard_msg_t> user_input_offboard_pub;
  /* Subs */
  OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  OVQueueSubscriber<SystemStatus_msg_t> system_status_sub;
  StaticJsonDocument<2048> jdoc;

};

void start_pc_comms_manager(void) {
  xDebugPrintSemaphore = xSemaphoreCreateMutex();
  PCCommsManager *ptr = new PCCommsManager();
  ptr->start();
}

void debug_print(char *buf) {
  if (xSemaphoreTake(xDebugPrintSemaphore, (TickType_t) 10) == pdTRUE) {
    uart_debug_tx_stream_idx++;
    UART_DEBUG_TX_STREAM[uart_debug_tx_stream_idx].buf = buf;
    if (uart_debug_tx_stream_idx > UART_DEBUG_TX_STREAM_LENGTH) {
      uart_debug_tx_stream_idx = 0;
    }
    xSemaphoreGive(xDebugPrintSemaphore);
  } else {
    /* For debugging purposes */
  }
}
