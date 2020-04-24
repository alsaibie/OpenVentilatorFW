#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;
using namespace OVTopics;
class ControllerManager : public OVThread {
 public:
  ControllerManager() :
      OVThread("Controller", 128, controller_m_priority, 10),
      actuator_cmd_pub(gActuatorCommandsOVQHandle),
      user_input_sub(
          gUserInputOVQHandle,
          std::bind(&ControllerManager::on_user_input_read, this,
                    std::placeholders::_1)),
      user_input_offboard_sub(
          gUserInputOffboardOVQHandle,
          std::bind(&ControllerManager::on_user_input_offboard_read, this,
                    std::placeholders::_1)),

      sensor_status_sub(
          gSensorStatusOVQHandle,
          std::bind(&ControllerManager::on_sensor_status_read, this,
                    std::placeholders::_1)),
      operation_status_sub(
          gOperationStatusOVQHandle,
          std::bind(&ControllerManager::on_operation_satus_read, this,
                    std::placeholders::_1)) {
  }

 protected:

  virtual void run() {

    while (1) {
      /*
       * From sensor data and based on mode, compute actuator cmd
       */
      user_input_sub.receive();
      user_input_offboard_sub.receive();
      sensor_status_sub.receive();
      operation_status_sub.receive();

      if (operationStatusMsg.input_source == UI::UserInputSource::Onboard) {
        switch (userInputMsg.system_mode) {
          case UI::UserSystem_Modes::Manual_Mode:
            // Test commanding position
            actuatorCmdMsg.primaryAxis.mode =
                Actuators::Actuator_Modes::Position;
            actuatorCmdMsg.primaryAxis.position = (int32_t)userInpuOffboardMsg
                .flow_sp_lpm;
            break;
          case UI::UserSystem_Modes::Pressure_Control:

            break;
          case UI::UserSystem_Modes::Pressure_Assist:

            break;
          case UI::UserSystem_Modes::Volume_Control:
            break;
          default:
            break;
        }
      } else if (operationStatusMsg.input_source
          == UI::UserInputSource::Offboard) {
        switch (userInpuOffboardMsg.system_mode) {
          case UI::UserSystem_Modes::Manual_Mode:
            // Test commanding position
            actuatorCmdMsg.primaryAxis.mode =
                Actuators::Actuator_Modes::Position;
            actuatorCmdMsg.primaryAxis.position = userInpuOffboardMsg
                .flow_sp_lpm;
            break;
          case UI::UserSystem_Modes::Pressure_Control:

            break;
          case UI::UserSystem_Modes::Pressure_Assist:

            break;
          case UI::UserSystem_Modes::Volume_Control:

            break;
          default:
            break;
        }
      }
      actuator_cmd_pub.publish(actuatorCmdMsg);
      thread_lap();
    }
  }

 private:
  void on_user_input_read(const UserInput_msg_t &msg) {
    userInputMsg = msg;
  }

  void on_user_input_offboard_read(const UserInputOffboard_msg_t &msg) {
    userInpuOffboardMsg = msg;
  }
  void on_sensor_status_read(const SensorStatus_msg_t &msg) {
    sensorStatusMsg = msg;
  }
  void on_operation_satus_read(const OperationStatus_msg_t &msg) {
    operationStatusMsg = msg;
  }

  /* Pubs */
  OVQueuePublisher<ActuatorCommands_msg_t> actuator_cmd_pub;
  ActuatorCommands_msg_t actuatorCmdMsg;
  /* Subs */
  OVQueueSubscriber<UserInput_msg_t> user_input_sub;
  UserInput_msg_t userInputMsg;
  OVQueueSubscriber<UserInputOffboard_msg_t> user_input_offboard_sub;
  UserInputOffboard_msg_t userInpuOffboardMsg;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  SensorStatus_msg_t sensorStatusMsg;
  OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
  OperationStatus_msg_t operationStatusMsg;
};

void start_controller_manager() {
  ControllerManager *ptr = new ControllerManager();
  ptr->start();
}
