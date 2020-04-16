#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

using namespace OVRTOS;
using namespace OVTopics;

class SystemMonitor : public OVThread {
   public:
    SystemMonitor()
        : OVThread("System Monitor", 128, system_mon_priority, 20),
          system_status_pub(gSystemStatusOVQHandle),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek),
          user_input_sub(gUserInputOVQHandle, &on_user_input_peek, Peek),
          safety_sub(gUserInputOVQHandle, &on_safety_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            /*
             * The system monitor publishes the overall state of the system,
             * it compares between the expected performance and the actual performance
             * and it issues deviations from the expected range.
             * */

            thread_lap();
        }
    }

   private:
    static void on_operation_status_peek(const OperationStatus_msg_t &msg) {}
    static void on_user_input_peek(const UserInput_msg_t &msg) {}
    static void on_safety_peek(const Safety_msg_t &msg) {}
    /* Pubs */
    OVQueuePublisher<SystemStatus_msg_t> system_status_pub;
    /* Subs */
    OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
    OVQueueSubscriber<UserInput_msg_t> user_input_sub;
    OVQueueSubscriber<Safety_msg_t> safety_sub;
};

void start_system_monitor(void) {
    SystemMonitor *ptr = new SystemMonitor();
    ptr->start();
}
