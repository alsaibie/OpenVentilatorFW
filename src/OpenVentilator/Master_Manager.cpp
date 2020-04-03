#include "OpenVentilator/OpenVentilator.hpp"

/* Include Topics */
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/operation_modes.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

/* Master Manager */
using namespace OVRTOS;

class MasterManager : public OVThread {
   public:
    MasterManager()
        : OVThread("Master Manager", 128, master_m_priority, 50),
          system_status_pub(gSensorStatusOVQHandle),
          operation_modes_pub(gOperationModesOVQHandle),
          safety_sub(gSafetyOVQHandle, &on_safety_receive, Receive),
          user_input_sub(gUserInputOVQHandle, &on_user_input_receive, Receive),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            safety_sub.receive();
            user_input_sub.receive();
            sensor_status_sub.receive();
            digitalToggle(LED_BUILTIN);

            OpModeMsg.a = 2;
            operation_modes_pub.publish(OpModeMsg);

            thread_lap();
        }
    }

   private:
    void update_system_state() {}

    void run_state_machine() {}

    static void on_user_input_receive(const OVTopics::UserInput_t &msg) {
        if (msg.sw == 3){
            Serial.println("User Input Msg Received");
        }
    }

    static void on_safety_receive(const OVTopics::Safety_t &msg) {
        if (msg.a == 1) {
            Serial.println("Safety Message Received");
        }
    }

    static void on_sensor_status_peek(const OVTopics::SensorStatus_t &msg) {
        if(msg.p == 4){
            Serial.println("Sensor Status Msg Received");
        }
    }

    /* Pubs */
    OVQueuePublisher<OVTopics::SystemStatus_t> system_status_pub;
    OVTopics::SystemStatus_t SysStatusMsg;
    OVQueuePublisher<OVTopics::OperationModes_t> operation_modes_pub;
    OVTopics::OperationModes_t OpModeMsg;

    /* Subs */
    OVQueueSubscriber<OVTopics::Safety_t> safety_sub;
    OVQueueSubscriber<OVTopics::UserInput_t> user_input_sub;
    OVQueueSubscriber<OVTopics::SensorStatus_t> sensor_status_sub;
};

void start_master_manager() {
    MasterManager *ptr = new MasterManager();
    ptr->start();
}

