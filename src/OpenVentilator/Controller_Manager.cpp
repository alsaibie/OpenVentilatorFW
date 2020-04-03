#include "OpenVentilator/OpenVentilator.hpp"

/* Publish Topics */
#include "OVTopics/actuator_commands.hpp"
/* Subscribe Topics */
#include "OVTopics/operation_modes.hpp"
#include "OVTopics/sensor_status.hpp"

using namespace OVRTOS;

class ControllerManager : public OVThread {
   public:
    ControllerManager()
        : OVThread("Controller", 128, controller_m_priority, 10),
          actuator_cmd_pub(gActuatorCommandsOVQHandle),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          operation_modes_sub(gOperationModesOVQHandle, &on_operation_modes_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            thread_lap();
        }
    }

   private:
    static void on_sensor_status_peek(const OVTopics::SensorStatus_t &msg) {}
    static void on_operation_modes_peek(const OVTopics::OperationModes_t &msg) {}

    /* Pubs */
    OVQueuePublisher<OVTopics::ActuatorCommands_t> actuator_cmd_pub;

    /* Subs */
    OVQueueSubscriber<OVTopics::SensorStatus_t> sensor_status_sub;
    OVQueueSubscriber<OVTopics::OperationModes_t> operation_modes_sub;
};

void start_controller_manager() {
    ControllerManager *ptr = new ControllerManager();
    ptr->start();
}