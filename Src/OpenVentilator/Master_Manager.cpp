#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

#include <cstdio>

/* Master Manager */
using namespace OVRTOS;
using namespace OVTopics;

/* Default Modes */
#define DEFAULT_OPERATION_STATE OperationState_Modes::Disarmed

class MasterManager : public OVThread {
 public:
  MasterManager() :
      OVThread("Master Manager", 128, master_m_priority, 50),
      system_status_pub(gSystemStatusOVQHandle),
      operation_status_pub(gOperationStatusOVQHandle),
      safety_sub(
          gSafetyOVQHandle,
          std::bind(&MasterManager::on_safety_read, this,
                    std::placeholders::_1)),
      user_input_sub(
          gUserInputOVQHandle,
          std::bind(&MasterManager::on_user_input_read, this,
                    std::placeholders::_1)),
      sensor_status_sub(
          gSensorStatusOVQHandle,
          std::bind(&MasterManager::on_sensor_status_read, this,
                    std::placeholders::_1)),
      operation_state(DEFAULT_OPERATION_STATE) {
  }

 protected:
  virtual void run() {
    while (1) {
      safety_sub.receive();
      user_input_sub.receive();
      sensor_status_sub.receive();

      OpStatusMsg.input_source = UI::UserInputSource::Onboard;
      OpStatusMsg.operation_state = operation_state;

      operation_status_pub.publish(OpStatusMsg);

      SysStatusMsg.Q.slpm = 10;

      system_status_pub.publish(SysStatusMsg);

      thread_lap();
    }
  }

 private:
  void update_system_state() {
  }

  void run_state_machine() {

  }

  void on_user_input_read(const OVTopics::UserInput_msg_t &msg) {
    //TODO: check

  }

  // TODO: for safety and similar queues, a peek will not work, need to read, or ackowledge persistent messages..
  void on_safety_read(const Safety_msg_t &msg) {
    if (msg.system_error == SystemErrors_Modes::Actuator_Error) {

    }

  }

  void on_sensor_status_read(const SensorStatus_msg_t &msg) {

  }

  /* Pubs */
  OVQueuePublisher<SystemStatus_msg_t> system_status_pub;
  SystemStatus_msg_t SysStatusMsg;
  OVQueuePublisher<OperationStatus_msg_t> operation_status_pub;
  OperationStatus_msg_t OpStatusMsg;

  /* Subs */
  OVQueueSubscriber<Safety_msg_t> safety_sub;
  Safety_msg_t safetyMsg;
  OVQueueSubscriber<UserInput_msg_t> user_input_sub;
  UserInput_msg_t userInputMsg;
  OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
  SensorStatus_msg_t sensorStatusMsg;

  OperationState_Modes operation_state;
};

void start_master_manager() {
  MasterManager *ptr = new MasterManager();
  ptr->start();
}

