#include "OpenVentilator/OpenVentilator.hpp"

#include "Device/UART.hpp"

/* Pubs */
#include "OVTopics/user_input_offboard.hpp"

/* Subs */
#include "OVTopics/operation_status.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

#include "semphr.h"

#include <cstring>

#include "ArduinoJson.h"

//#define DEBUG_PRINT

using namespace OVRTOS;
using namespace OVTopics;
SemaphoreHandle_t xDebugPrintSemaphore = NULL;

typedef struct _STREAM {
  static size_t const buffer_size{200};
  char buf[buffer_size];
  size_t len{200};
} STREAM;

#define UART_DEBUG_TX_STREAM_LENGTH 15
STREAM UART_DEBUG_TX_STREAM[UART_DEBUG_TX_STREAM_LENGTH];

int16_t uart_debug_tx_stream_idx = -1;

//#define clear_stream(stream) {(stream).buf = NULL; (stream).len = 0;}
void clear_stream(STREAM &stream){
  memset(stream.buf, 0, stream.len);
  stream.len = 0;
}

#define vcom_huart huart3
extern UART_HandleTypeDef vcom_huart;

#define ECHO_TEST false

class VCUART : public UART {
 public:
  VCUART(STREAM &inc_stream) :
      UART(vcom_huart),
      incoming_str(inc_stream) {
    clear_stream(incoming_str);
  }

  virtual void process_line(const uint8_t *buf, size_t len) {

#if ECHO_TEST
     tx_dma_buffer(buf, len);
     tx_dma_buffer((uint8_t*)"\n", 1);
 #endif
     for (int k = 0; k < len; k++){
       incoming_str.buf[k] = (char)buf[k];
       //Need to copy so original is cleared immediatly
     }
    incoming_str.len = len;
  }

 private:
  STREAM &incoming_str;

};

class PCCommsManager : public OVThread {
 public:
  PCCommsManager() :
      OVThread("PC Communications", 2056, pc_comms_m_priority, 100),
      /* std::bind is required to attach non-static callback functions to constructor, this may not be needed, I could as well have static functions */
      user_input_offboard_pub(gUserInputOffboardOVQHandle),
      operation_status_sub(
          gOperationStatusOVQHandle,
          std::bind(&PCCommsManager::on_operation_status_read, this,
                    std::placeholders::_1)),
      sensor_status_sub(
          gSensorStatusOVQHandle,
          std::bind(&PCCommsManager::on_sensor_status_read, this,
                    std::placeholders::_1)),
      system_status_sub(
          gSystemStatusOVQHandle,
          std::bind(&PCCommsManager::on_system_status_read, this,
                    std::placeholders::_1)),
      user_input_sub(
          gUserInputOVQHandle,
          std::bind(&PCCommsManager::on_user_input_read, this,
                    std::placeholders::_1)),
      VComUART(incoming_rx_stream) {
  }

 protected:
  virtual void run() final {
    VComUART.start();

    ui_ob_msg.system_mode = UI::UserSystem_Modes::Manual_Mode;
    clear_stream(incoming_rx_stream);
    vTaskDelay(100);

    while (1) {
      VComUART.spin_circular();
      /* Receive and relay states to PC */
      operation_status_sub.receive();
      sensor_status_sub.receive();
      system_status_sub.receive();
      user_input_sub.receive();

      /* Receive Commands from PC */
      if (incoming_rx_stream.len != 0) {
        /* Process new line TODO: make a stream array to handle a backlog */
        const size_t capacity = JSON_OBJECT_SIZE(5);
        StaticJsonDocument<capacity> doc;

        deserializeJson(doc, incoming_rx_stream.buf);

        /* Relay to OVTopic */
        ui_ob_msg.flow_sp_lpm = doc["val"];
        if(int(doc["val"])!=0){
          HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
          ui_ob_msg.rate_sp_hz = 10;
        }
//        ui_ob_msg.flow_sp_lpm = doc["flow_lpm"];

        ui_ob_msg.IE_ratio = doc["ie_ratio"];

        ui_ob_msg.system_mode =
            static_cast<UI::UserSystem_Modes>(static_cast<int>(doc["system_mode"]));

        if(user_input_offboard_pub.publish(ui_ob_msg) != pdPASS){
          /* why? */
          int a = 1;
        }

        clear_stream(incoming_rx_stream);

      }

#ifdef DEBUG_PRINT
			/* Forward debug tx messages TOOD: remove after embedding debug into JSON */
			tx_debug_messages();
#endif
      thread_lap();
    }
  }

 private:
  VCUART VComUART;
  static STREAM incoming_rx_stream;


  void tx_debug_messages() {
    if (xSemaphoreTake(xDebugPrintSemaphore, (TickType_t) 10) == pdTRUE) {
      while (uart_debug_tx_stream_idx >= 0) {

        VComUART.print(UART_DEBUG_TX_STREAM[uart_debug_tx_stream_idx].buf);

        clear_stream(UART_DEBUG_TX_STREAM[uart_debug_tx_stream_idx]);

        uart_debug_tx_stream_idx--;

      }

      xSemaphoreGive(xDebugPrintSemaphore);
    } else {
      /* For debugging purposes */
    }
  }

  void on_operation_status_read(const OperationStatus_msg_t &msg) {
    /* relay messages to PC */
    const size_t capacity = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;
    doc["T"] = HAL_GetTick();
    doc["S"] = "OpStatus";
    doc["OpState"] = (int) msg.operation_state;
    serializeJson(doc, VComUART);
    VComUART.EOL();
    vTaskDelay(10);
  }

  void on_sensor_status_read(const SensorStatus_msg_t &msg) {
    /* relay messages to PC */
    const size_t capacity = JSON_OBJECT_SIZE(6);
    StaticJsonDocument<capacity> doc;

    doc["T"] = HAL_GetTick();
    doc["S"] = "SensorStatus";
    doc["P"] = msg.P_mmH2O;
    doc["Q"] = msg.Q_SLPM;

    serializeJson(doc, VComUART);
    VComUART.EOL();
    vTaskDelay(10);
  }

  void on_system_status_read(const SystemStatus_msg_t &msg) {
    /* relay messages to PC */
    const size_t capacity = JSON_OBJECT_SIZE(10);
    StaticJsonDocument<capacity> doc;

    doc["T"] = msg.system.uptime;
    doc["S"] = "SystemStatus";
    doc["P"][0] = msg.P.average;
    doc["P"][1] = msg.P.min;
    doc["P"][2] = msg.P.max;
    doc["P"][3] = msg.P.exp;
    doc["V"][0] = msg.V.average;
    doc["V"][1] = msg.V.min;
    doc["V"][2] = msg.V.max;
    doc["Q"] = msg.Q.slpm;

    serializeJson(doc, VComUART);
    VComUART.EOL();
    vTaskDelay(10);
  }
  void on_user_input_read(const UserInput_msg_t &msg) {
    const size_t capacity = JSON_OBJECT_SIZE(10);
    StaticJsonDocument<capacity> doc;

    doc["T"] = HAL_GetTick();
    doc["S"] = "UserInputs";
    doc["RateSp"] = msg.rate_sp_hz;
    doc["FlowSp"] = msg.flow_sp_lpm;
    doc["IESp"] = msg.IE_ratio;
    doc["SysMode"] = (int) msg.system_mode;

    serializeJson(doc, VComUART);
    VComUART.EOL();
    vTaskDelay(10);
  }

  /* Pubs */
  OVQueuePublisher<UserInputOffboard_msg_t> user_input_offboard_pub;
  UserInputOffboard_msg_t ui_ob_msg;
  /* Subs */
  OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  OVQueueSubscriber<SystemStatus_msg_t> system_status_sub;
  OVQueueSubscriber<UserInput_msg_t> user_input_sub;
};

STREAM PCCommsManager::incoming_rx_stream;

void start_pc_comms_manager(void) {
  xDebugPrintSemaphore = xSemaphoreCreateMutex();
  PCCommsManager *ptr = new PCCommsManager();
  ptr->start();
}

void debug_print(char *buf) {
#ifdef DEBUG_PRINT
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
#endif
}
