#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;
using namespace OVTopics;
class SafetyManager : public OVThread {
 public:
  SafetyManager() :
      OVThread("Safety Manager", 128, safety_m_priority, 50),
      safety_pub(gSafetyOVQHandle),
      user_input_sub(
          gUserInputOVQHandle,
          std::bind(&SafetyManager::on_user_input_read, this,
                    std::placeholders::_1)),
      sensor_status_sub(
          gSensorStatusOVQHandle,
          std::bind(&SafetyManager::on_sensor_status_read, this,
                    std::placeholders::_1)),
      operation_status_sub(
          gOperationStatusOVQHandle,
          std::bind(&SafetyManager::on_operation_status_read, this,
                    std::placeholders::_1)) {
  }

 protected:
  virtual void run() {
    /*
     * Initially: The safety manager should check the sensor readings are within
     * the expected range, and also respond to emergency stop. It should issue commands directly.
     * The safety manager should behave like a watchdog, making sure the tasks are running and messages
     * are published
     * The task should also check the power level in and power level to the actuators
     *
     */

    while (1) {
      user_input_sub.receive();
      sensor_status_sub.receive();
      operation_status_sub.receive();

      safety_msg.system_error = SystemErrors_Modes::Flow_Sensor_Error;
      safety_pub.publish(safety_msg);
//            debug_print("Safety Manager Thread\n");
      thread_lap();
    }
  }

 private:
  void on_sensor_status_read(const SensorStatus_msg_t &msg) {

  }

  void on_user_input_read(const UserInput_msg_t &msg) {
  }

  void on_operation_status_read(
      const OVTopics::OperationStatus_msg_t &msg) {
    if (msg.operation_state == OperationState_Modes::Disarmed) {
      // Serial.println("System Disarmed");
    }
  }

  /* Pubs */
  OVQueuePublisher<OVTopics::Safety_msg_t> safety_pub;
  OVTopics::Safety_msg_t safety_msg;
  /* Subs */
  OVQueueSubscriber<UserInput_msg_t> user_input_sub;
  UserInput_msg_t userInputMsg;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  SensorStatus_msg_t sensorStatusMsg;
  OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
  OperationStatus_msg_t operationStatusMsg;
  /* */
};

void start_safety_manager(void) {
  SafetyManager *ptr = new SafetyManager();
  ptr->start();
}
