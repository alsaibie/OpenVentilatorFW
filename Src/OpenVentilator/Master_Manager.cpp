#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"


/* Master Manager */
using namespace OVRTOS;
using namespace OVTopics;

/* Default Modes */
#define DEFAULT_OPERATION_STATE OperationState_Modes::DISARMED

class MasterManager : public OVThread {
   public:
    MasterManager()
        : OVThread("Master Manager", 128, master_m_priority, 50),
          system_status_pub(gSensorStatusOVQHandle),
          operation_status_pub(gOperationStatusOVQHandle),
          safety_sub(gSafetyOVQHandle, &on_safety_receive, Receive),
          user_input_sub(gUserInputOVQHandle, &on_user_input_peek, Peek),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          operation_state(DEFAULT_OPERATION_STATE)
           {}

   protected:
    virtual void run() {
        while (1) {
            safety_sub.receive();
            user_input_sub.receive();
            sensor_status_sub.receive();
            

//            debug_print("Master Manager Thread\n");

            OpStatusMsg.operation_state = operation_state;

            operation_status_pub.publish(OpStatusMsg);

            SysStatusMsg.Q.slpm = 10;

            system_status_pub.publish(SysStatusMsg);

            thread_lap();
        }
    }

   private:
    void update_system_state() {}

    void run_state_machine() {


    }

    static void on_user_input_peek(const OVTopics::UserInput_msg_t &msg) {
            // Serial.print("Flow Rate: ");
            // Serial.print(msg.flow_sp_lpm);
            // Serial.print(" Rate: ");
            // Serial.print(msg.rate_sp_hz);
            // Serial.print(" IE Ratio: ");
            // Serial.print(msg.IE_ratio);
            // Serial.println("");
    	int a = 1;
    }

    static void on_safety_receive(const Safety_msg_t &msg) {
    	//TODO: if a queue is received and not peeked, warn on peeking since it might not be available to read. or use stream instead.
        if (msg.system_error == SystemErrors_Modes::ACTUATOR_ERROR) {
            // Serial.println("Safety Message Received");
        }
    }

    static void on_sensor_status_peek(const SensorStatus_msg_t &msg) {
        if(msg.R_Hz == 4){
            // Serial.println("Sensor Status Msg Received");
        }
    }

    /* Pubs */
    OVQueuePublisher<SystemStatus_msg_t> system_status_pub;
    SystemStatus_msg_t SysStatusMsg;
    OVQueuePublisher<OperationStatus_msg_t> operation_status_pub;
    OperationStatus_msg_t OpStatusMsg;

    /* Subs */
    OVQueueSubscriber<Safety_msg_t> safety_sub;
    OVQueueSubscriber<UserInput_msg_t> user_input_sub;
    OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;

    OperationState_Modes operation_state;
};

void start_master_manager() {
    MasterManager *ptr = new MasterManager();
    ptr->start();
}

