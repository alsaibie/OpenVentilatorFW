#include "OpenVentilator/OpenVentilator.hpp"
#include "Actuators/LinearAxis.hpp"

/* Pub Topics */
// Actuator Status?

/* Sub Topics */
#include "OVTopics/actuator_commands.hpp"

using namespace OVRTOS;

class ActuatorManager : public OVThread {
   public:
    ActuatorManager()
        : OVThread("Actuator Manager ", 128, actuator_m_priority, 20),
          actuator_cmd_sub(gActuatorCommandsOVQHandle, &on_actuator_cmd_receive, Receive) {}

   protected:
    virtual void run() {
        while (1) {
            thread_lap();
        }
    }

   private:
    static void on_actuator_cmd_receive(const OVTopics::ActuatorCommands_msg_t &msg) {
        
    }
    /* Pubs */

    /* Subs */
    OVQueueSubscriber<OVTopics::ActuatorCommands_msg_t> actuator_cmd_sub;
};

void start_actuator_manager() {
    ActuatorManager *ptr = new ActuatorManager();
    ptr->start();
}