#include "Actuators/RotatingAxis.hpp"
#include "board_configuration.hpp"
#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"

using namespace OVRTOS;
using namespace OVTopics;
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

    static RotatingAxis *primaryAxis;

   protected:
    virtual void run() {
        uint32_t last_millis = HAL_GetTick();
        while (1) {
            /*
             * Upon receiving the command from the controller. Any actuator mode changes
             * will be done, and the corresponding setpoint for each will be set.
             * The spinAxis call is a synchronous call to command the motors directly
             * */

            primaryAxis->spinAxis(HAL_GetTick() - last_millis);

            last_millis = HAL_GetTick();

            actuator_cmd_sub.receive();

            thread_lap();
        }
    }

   private:
    static void on_actuator_cmd_receive(const OVTopics::ActuatorCommands_msg_t &msg) {
        static Actuator_Modes mode = Actuator_Modes::Velocity;

        // TODO: Include other modes
        Actuator_Modes new_mode = msg.primaryAxis.mode;
        if (new_mode != mode) {
            primaryAxis->setMode(new_mode);
            mode = new_mode;
        }

        if (mode == Actuator_Modes::Position) {
            primaryAxis->setVelocity(msg.primaryAxis.position);
        } else if (mode == Actuator_Modes::Velocity) {
            primaryAxis->setVelocity(msg.primaryAxis.speed);
        } else if (mode == Actuator_Modes::Torque) {
            primaryAxis->setTorque(msg.primaryAxis.torque);
        } else if (mode == Actuator_Modes::PositionTrajectory || mode == Actuator_Modes::VelocityTrajectory ||
                   mode == Actuator_Modes::TorqueTrajectory) {
          primaryAxis->updateTrajectory(msg.primaryAxis.trajectory);
        }
    }
    /* Pubs */

    /* Subs */
    OVQueueSubscriber<OVTopics::ActuatorCommands_msg_t> actuator_cmd_sub;
};

RotatingAxis *ActuatorManager::primaryAxis;

void start_actuator_manager() {
    /* Configure Primary Actuator */
    StepperPinConfig_t stp_config{MOTOR_LEFT_ENABLE_PIN, MOTOR_LEFT_DIR_PIN, MOTOR_LEFT_STEP_PIN};

    Stepper primaryStepper("Primary Stepper", stp_config);
    RotatingAxis_config_t rotax_cfg({prAxisGearRatio, prAxisMotionRange, open_limit_pos, prAxisMaxPos,
                                                prAxisMinPos, prAxisMaxVel, prAxisMaxAcc, prAxisMinAcc});
    ActuatorManager::primaryAxis = new RotatingAxis("Primary Axis", rotax_cfg, primaryStepper);

    ActuatorManager *ptr = new ActuatorManager();

    ptr->start();
}
