#include "OpenVentilator/OpenVentilator.hpp"

/* Publish Topics */
#include "OVTopics/actuator_commands.hpp"
/* Subscribe Topics */
#include "OVTopics/user_input.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/sensor_status.hpp"

using namespace OVRTOS;

class ControllerManager : public OVThread {
   public:
    ControllerManager()
        : OVThread("Controller", 128, controller_m_priority, 10),
          actuator_cmd_pub(gActuatorCommandsOVQHandle),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_modes_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            /*
            * From sensor data and based on mode, compute actuator cmd 
            */

           /* Mode 1: Pressure Control */


            /* Mode 2: Pressure Control - Breath Assist */

            /* Mode 3: Volume Control */

            /* Mode 4: ... */
            
            thread_lap();
        }
    }

   private:
    static void on_sensor_status_peek(const OVTopics::SensorStatus_msg_t &msg) {}
    static void on_operation_modes_peek(const OVTopics::OperationStatus_msg_t &msg) {}

    /* Pubs */
    OVQueuePublisher<OVTopics::ActuatorCommands_msg_t> actuator_cmd_pub;

    /* Subs */
    OVQueueSubscriber<OVTopics::SensorStatus_msg_t> sensor_status_sub;
    OVQueueSubscriber<OVTopics::OperationStatus_msg_t> operation_status_sub;
};

void start_controller_manager() {
    ControllerManager *ptr = new ControllerManager();
    ptr->start();
}