#include "OpenVentilator/OpenVentilator.hpp"

/* OVTopics */
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/sensor_status.hpp"

using namespace OVRTOS;

class EmulatorManager : public OVThread {
   public:
    EmulatorManager()
        : OVThread("Emulator", 128, emulator_m_priority, 20),
          sensor_status_pub(gSensorStatusOVQHandle),
          actuator_cmd_sub(gActuatorCommandsOVQHandle, &on_actuator_commands_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            
            thread_lap();
        }
    }
   private:
    static void on_actuator_commands_peek(const OVTopics::ActuatorCommands_msg_t &msg) {}

    /* Pubs */
    OVQueuePublisher<OVTopics::SensorStatus_msg_t> sensor_status_pub;
    OVTopics::SensorStatus_msg_t SensorMsg;
    /* Subs */
    OVQueueSubscriber<OVTopics::ActuatorCommands_msg_t> actuator_cmd_sub;
};

void start_emulator_manager() {
    EmulatorManager *ptr = new EmulatorManager();
    ptr->start();
}