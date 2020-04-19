#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;
using namespace OVTopics;

#include <cmath>

class SystemMonitor : public OVThread {
   public:
    SystemMonitor()
        : OVThread("System Monitor", 256, system_mon_priority, 20),
          system_status_pub(gSystemStatusOVQHandle),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek),
          user_input_sub(gUserInputOVQHandle, &on_user_input_peek, Peek),
          safety_sub(gUserInputOVQHandle, &on_safety_peek, Peek),
		  sysStatus_msg{0} {}

   protected:
    virtual void run() {

        while (1) {
            /*
             * The system monitor publishes the overall state of the system,
             * it compares between the expected performance and the actual performance
             * and it issues deviations from the expected range.
             * */
        	uint32_t t = HAL_GetTick();
        	sysStatus_msg.P.average = 3*sin(t/2000.0);
        	sysStatus_msg.system.uptime = HAL_GetTick();
        	system_status_pub.publish(sysStatus_msg);
        	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            thread_lap();
        }

        int a = 1;
    }

   private:
    static void on_operation_status_peek(const OperationStatus_msg_t &msg) {}
    static void on_user_input_peek(const UserInput_msg_t &msg) {}
    static void on_safety_peek(const Safety_msg_t &msg) {}
    /* Pubs */
    OVQueuePublisher<SystemStatus_msg_t> system_status_pub;
    SystemStatus_msg_t sysStatus_msg;
    /* Subs */
    OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
    OVQueueSubscriber<UserInput_msg_t> user_input_sub;
    OVQueueSubscriber<Safety_msg_t> safety_sub;
};

void start_system_monitor(void) {
    SystemMonitor *ptr = new SystemMonitor();
    ptr->start();
}
