#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/sensor_status.hpp"

using namespace OVRTOS;
using namespace OVTopics;
class EmulatorManager : public OVThread {
 public:
  EmulatorManager() :
      OVThread("Emulator", 128, emulator_m_priority, 20),
      sensor_status_pub(gSensorStatusOVQHandle),
      actuator_cmd_sub(
          gActuatorCommandsOVQHandle,
          std::bind(&EmulatorManager::on_actuator_commands_read, this,
                    std::placeholders::_1)) {
  }

 protected:
  virtual void run() {
    while (1) {
      /*
       * The emulator is the digital twin of the ventilator device
       * It will run whether in SITL or Op Mode.
       * It is the controller's job to choose whether to use the emulator ouput
       * or actual sensors output. The emulator should mimick only what the sensors can give
       * */

      thread_lap();
    }
  }
 private:
  void on_actuator_commands_read(const ActuatorCommands_msg_t &msg) {
  }

  /* Pubs */
  OVQueuePublisher<SensorStatus_msg_t> sensor_status_pub;
  SensorStatus_msg_t SensorMsg;
  /* Subs */
  OVQueueSubscriber<ActuatorCommands_msg_t> actuator_cmd_sub;
  ActuatorCommands_msg_t actuatorCmdMsg;
};

void start_emulator_manager() {
  EmulatorManager *ptr = new EmulatorManager();
  ptr->start();
}
