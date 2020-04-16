#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;
using namespace OVTopics;
class SafetyManager : public OVThread {
   public:
    SafetyManager()
        : OVThread("Safety Manager", 128, safety_m_priority, 50),
          safety_pub(gSafetyOVQHandle),
          user_input_sub(gUserInputOVQHandle, &on_user_input_peek, Peek),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek) {}

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

            safety_msg.system_error = SystemErrors_Modes::FLOW_SENSOR_ERROR;
            safety_pub.publish(safety_msg);
            debug_print("Safety Manager Thread\n");
            thread_lap();
        }
    }

   private:
    static void on_sensor_status_peek(const SensorStatus_msg_t &msg) {

    }

    static void on_user_input_peek(const UserInput_msg_t &msg) {}

    static void on_operation_status_peek(const OVTopics::OperationStatus_msg_t &msg) {
        if (msg.operation_state == OperationState_Modes::DISARMED) {
            // Serial.println("System Disarmed");
        }
    }

    /* Pubs */
    OVQueuePublisher<OVTopics::Safety_msg_t> safety_pub;
    OVTopics::Safety_msg_t safety_msg;
    /* Subs */
    OVQueueSubscriber<UserInput_msg_t> user_input_sub;
    OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
    OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
};

void start_safety_manager(void) {
    SafetyManager *ptr = new SafetyManager();
    ptr->start();
}