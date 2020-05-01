#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"
#include "OVTopics/sensor_status.hpp"

using namespace OVRTOS;
using namespace OVTopics;

#include <cmath>

class SystemMonitor : public OVThread {
 public:
  SystemMonitor() :
      OVThread("System Monitor", 256, system_mon_priority, 20),
      system_status_pub(gSystemStatusOVQHandle),
      operation_status_sub(
          gOperationStatusOVQHandle,
          std::bind(&SystemMonitor::on_operation_status_read, this,
                    std::placeholders::_1)),
      user_input_sub(
          gUserInputOVQHandle,
          std::bind(&SystemMonitor::on_user_input_read, this,
                    std::placeholders::_1)),
      safety_sub(
          gUserInputOVQHandle,
          std::bind(&SystemMonitor::on_safety_read, this,
                    std::placeholders::_1)),
      sensor_status_sub(
          gUserInputOVQHandle,
          std::bind(&SystemMonitor::on_sensor_status_read, this,
                    std::placeholders::_1)) {}

 protected:
  virtual void run() {

    while (1) {
      /*
       * The system monitor publishes the overall state of the system,
       * it compares between the expected performance and the actual performance
       * and it issues deviations from the expected range.
       * */


      // as a start, just relay the sensor info back


      uint32_t t = HAL_GetTick();
      sysStatus_msg.P.average = 3 * sin(t / 2000.0);
      sysStatus_msg.system.uptime = HAL_GetTick();
      system_status_pub.publish(sysStatus_msg);
      HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
      thread_lap();
    }

  }

 private:

  void on_operation_status_read(const OperationStatus_msg_t &msg) {
    operationStatusMsg = msg;
  }
  void on_user_input_read(const UserInput_msg_t &msg) {
    userInputMsg = msg;
  }
  void on_safety_read(const Safety_msg_t &msg) {
    safetyMsg = msg;
  }
  void on_sensor_status_read(const SensorStatus_msg_t &msg) {
    sensorStatusMsg = msg;
  }

  /* Pubs */
  OVQueuePublisher<SystemStatus_msg_t> system_status_pub;
  SystemStatus_msg_t sysStatus_msg;

  /* Subs */
  OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
  OperationStatus_msg_t operationStatusMsg;
  OVQueueSubscriber<UserInput_msg_t> user_input_sub;
  UserInput_msg_t userInputMsg;
  OVQueueSubscriber<Safety_msg_t> safety_sub;
  Safety_msg_t safetyMsg;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  SensorStatus_msg_t sensorStatusMsg;

};

void start_system_monitor(void) {
  SystemMonitor *ptr = new SystemMonitor();
  ptr->start();
}
