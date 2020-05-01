#include "Actuators/RotatingAxis.hpp"
#include "board_configuration.hpp"
#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/actuator_commands.hpp"

//#include "Actuators/pwm_stepper.h"

#include "task.h"

using namespace OVRTOS;
using namespace OVTopics;
using namespace Actuators;

/* Declare stepper global to access update method in interrupt handler */
Stepper *primaryStepper;
RotatingAxis *primaryAxis;

extern TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigOCtim4 = { 0 };

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
  ActuatorManager() :
      OVThread("Actuator Manager ", 128, actuator_m_priority, 20),
      actuator_cmd_sub(
          gActuatorCommandsOVQHandle,
          std::bind(&ActuatorManager::on_actuator_cmd_read, this,
                    std::placeholders::_1)) {
  }

   virtual ~ActuatorManager(){

   }
 protected:
  virtual void run() {

    primaryStepper->enableMotor();

    while (1) {
      /*
       * Upon receiving the command from the controller. Any actuator mode changes
       * will be done, and the corresponding setpoint for each will be set.
       * The spinAxis call is a synchronous call to command the motors directly
       * */
      actuator_cmd_sub.receive();

//      test_routine();

      switch (actuatorCmdMsg.primaryAxis.mode) {
        case Actuator_Modes::Position:
          primaryStepper->moveToStepPosition(
              actuatorCmdMsg.primaryAxis.position);
          break;
        case Actuator_Modes::Velocity:
          break;
        case Actuator_Modes::Torque:
          break;
        case Actuator_Modes::PositionTrajectory:
          break;
        case Actuator_Modes::VelocityTrajectory:
          break;
        case Actuator_Modes::TorqueTrajectory:
          break;
        default:
          break;
      }

      thread_lap();
    }
  }

 private:
  void test_routine() {
    primaryStepper->moveSteps(800);
    vTaskDelay(4000);
    primaryStepper->moveSteps(-800);
    vTaskDelay(4000);
    primaryStepper->moveSteps(800);
    vTaskDelay(4000);
    primaryStepper->moveSteps(-800);
    vTaskDelay(4000);
  }

  void on_actuator_cmd_read(const OVTopics::ActuatorCommands_msg_t &msg) {
    actuatorCmdMsg = msg;
  }
  /* Pubs */

  /* Subs */
  OVQueueSubscriber<ActuatorCommands_msg_t> actuator_cmd_sub;
  ActuatorCommands_msg_t actuatorCmdMsg;
};

static StepperConfig_t stp_cfg = { .acc = 2000, .v_cruise = 1200, .v_min_start =
    150, .v_min_stop = 150, .steps_per_rev = 400 };
static StepperPinsConfig_t stp_pin_cfg = {
    .pwm = { M1PWM_GPIO_Port, M1PWM_Pin },
    .dir = { M1DIR_GPIO_Port, M1DIR_Pin }, .en = { M1EN_GPIO_Port, M1EN_Pin },
    .ms1 = { NULL, NULL }, .ms2 = { NULL, NULL }, .ms3 = { NULL, NULL } };

static RotatingAxis_config_t rotax_cfg( { prAxisGearRatio, prAxisMotionRange,
open_limit_pos, prAxisMaxPos,
prAxisMinPos, prAxisMaxVel, prAxisMaxAcc, prAxisMinAcc });

void start_actuator_manager() {
  /* Configure Primary Actuator */

  primaryStepper = new Stepper("Primary Stepper", stp_cfg, stp_pin_cfg, htim4,
  TIM_CHANNEL_4,
                               sConfigOCtim4, 1E5);

  primaryAxis = new RotatingAxis("Primary Axis", rotax_cfg, *primaryStepper);
  ActuatorManager *ptr = new ActuatorManager();

  ptr->start();
}

static void tim4_ch4_pulse_update_callback(TIM_HandleTypeDef *htim) {
  primaryStepper->pulseUpdate();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM4) {
    tim4_ch4_pulse_update_callback(htim);
  }
}
