#include "OpenVentilator/OpenVentilator.hpp"

/* OVTopics */
#include "OVTopics/operation_modes.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;

class SafetyManager : public OVThread {
   public:
    SafetyManager()
        : OVThread("Safety Manager", 128, safety_m_priority, 50),
          safety_pub(gSafetyOVQHandle),
          user_input_sub(gUserInputOVQHandle, &on_user_input_receive, Receive),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          operation_modes_sub(gOperationModesOVQHandle, &on_operation_modes_peek, Peek) {}

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
            operation_modes_sub.receive();

            SafetyMsg.a = 1;
            safety_pub.publish(SafetyMsg);

            thread_lap();
        }
    }

   private:
    static void on_sensor_status_peek(const OVTopics::SensorStatus_t &msg) {}

    static void on_user_input_receive(const OVTopics::UserInput_t &msg) {}

    static void on_operation_modes_peek(const OVTopics::OperationModes_t &msg) {
        if (msg.a == 2) {
            Serial.println("Op Mode Message Received");
        }
    }

    /* Pubs */
    OVQueuePublisher<OVTopics::Safety_t> safety_pub;
    OVTopics::Safety_t SafetyMsg;
    /* Subs */
    OVQueueSubscriber<OVTopics::UserInput_t> user_input_sub;
    OVQueueSubscriber<OVTopics::SensorStatus_t> sensor_status_sub;
    OVQueueSubscriber<OVTopics::OperationModes_t> operation_modes_sub;
};

void start_safety_manager(void) {
    SafetyManager *ptr = new SafetyManager();
    ptr->start();
}