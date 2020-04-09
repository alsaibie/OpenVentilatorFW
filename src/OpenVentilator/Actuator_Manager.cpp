#include "Actuators/RotatingAxis.hpp"
#include "OpenVentilator/OpenVentilator.hpp"
#include "board_configuration.hpp"
/* Pub Topics */
// Actuator Status?

/* Sub Topics */
#include "OVTopics/actuator_commands.hpp"

using namespace OVRTOS;
using namespace Actuators;

#define prAxisGearRatio 3
#define prAxisMotionRange 3
#define open_limit_pos 0
#define prAxisMaxPos 3
#define prAxisMinPos 0
#define prAxisMaxVel 3
#define prAxisMaxAcc 3
#define prAxisMinAcc 3
class ActuatorManager : public OVThread {
   public:
    ActuatorManager()
        : OVThread("Actuator Manager ", 128, actuator_m_priority, 20),
          actuator_cmd_sub(gActuatorCommandsOVQHandle, &on_actuator_cmd_receive, Receive) {}

    static Actuators::RotatingAxis *primaryAxis;

   protected:
    virtual void run() {
        time_t last_millis = millis();
        while (1) {
            /*
             * Upon receiving the command, and based on the operating mode the
             * the commands will be relayed to the actuator.
             *
             * */

            primaryAxis->spinAxis(millis() - last_millis);
            last_millis = millis();

            actuator_cmd_sub.receive();

            thread_lap();
        }
    }

   private:
    void test() {}
    static void on_actuator_cmd_receive(const OVTopics::ActuatorCommands_msg_t &msg) {
        static Actuators::Actuator_modes_t mode = Actuators::Velocity;

        // TODO: Include other modes
        Actuators::Actuator_modes_t new_mode = msg.primaryAxis.mode;
        if (new_mode != mode) {
            primaryAxis->setMode(new_mode);
            mode = new_mode;
        }

        if (mode == Actuators::Position) {
            primaryAxis->setVelocity(msg.primaryAxis.position);
        } else if (mode == Actuators::Velocity) {
            primaryAxis->setVelocity(msg.primaryAxis.speed);
        } else if (mode == Actuators::Torque) {
            primaryAxis->setTorque(msg.primaryAxis.torque);
        }
    }
    /* Pubs */

    /* Subs */
    OVQueueSubscriber<OVTopics::ActuatorCommands_msg_t> actuator_cmd_sub;

};

RotatingAxis *ActuatorManager::primaryAxis;

void start_actuator_manager() {

    /* Configure Primary Actuator */
    Actuators::StepperPinConfig_t stp_config{MOTOR_LEFT_ENABLE_PIN, MOTOR_LEFT_DIR_PIN, MOTOR_LEFT_STEP_PIN};

    Actuators::Stepper primaryStepper("Primary Stepper", stp_config);
    Actuators::RotatingAxis_config_t rotax_cfg({prAxisGearRatio, prAxisMotionRange, open_limit_pos, prAxisMaxPos,
                                                prAxisMinPos, prAxisMaxVel, prAxisMaxAcc, prAxisMinAcc});
    ActuatorManager::primaryAxis = new RotatingAxis("Primary Axis", rotax_cfg, primaryStepper);


    ActuatorManager *ptr = new ActuatorManager();
    
    ptr->start();
}