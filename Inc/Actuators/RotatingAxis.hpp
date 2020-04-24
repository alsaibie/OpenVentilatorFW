#ifndef ROTATINGAXIS_HPP_
#define ROTATINGAXIS_HPP_

/*TODO: A rotating axis is almost identical to a linear one,
 but this may change as implementation progresses */
#include "OVTopics/common_types.hpp"
#include "Stepper.hpp"
namespace Actuators {
typedef struct {
  uint32_t gear_ratio; /* [RPS to RPS] */
  uint32_t motion_range; /* [rad] */
  int32_t lower_limit_pos; /* [rad] */
  int32_t max_pos; /* [rad] */
  int32_t min_pos; /* [rad] */
  uint32_t max_velocity; /* [rad/s] */
  int32_t max_acc; /* [rad/s^2] */
  int32_t min_acc; /* [rad/s^2] */

} RotatingAxis_config_t;

// typedef enum { Reverse = 0, Forward } MotorDirection_t;
class RotatingAxis {
 public:
  RotatingAxis(const char *name, RotatingAxis_config_t &cfg, Stepper &mot) :
    control_mode(Actuator_Modes::Position), config(cfg), position_sp(0), velocity_sp(0), torque_sp(0),
    position_est(0), velocity_est(0), acceleration_est(0),
      motor(mot),
      currently_homing(false),
      home_reached(false) {
    axis_name = name;
  }

  /* Periodic Motor Control Call */
  void spinAxis(uint32_t dt_us);

  int32_t stepsFromPosition(int32_t x_sp, uint32_t dt_us);
  int32_t stepsFromVelocity(int32_t x_sp, uint32_t dt_us);
  int32_t stepsFromTorque(int32_t x_sp, uint32_t dt_us);

  inline void homeAxis() {
    currently_homing = true;
  }
  ;

  /* Set Actuator Control Mode */

  inline void setMode(const Actuator_Modes &m) {
    control_mode = m;
  }

  inline void setPosition(const int32_t &p) {
    position_sp = p;
  }

  inline void setVelocity(const int32_t &v) {
    velocity_sp = v;
  }

  inline void setTorque(const int32_t &tau) {
    torque_sp = tau;
  }

  /* Note: the spinAxis method will hold and execute a copy of the trajectory and
   * determine when to use the updated traj
   * */
  inline void updateTrajectory(const AxisTrajectory_t &traj_) {
    Traj = traj_;
  }

 private:
  Actuator_Modes control_mode;
  RotatingAxis_config_t config;
  int32_t position_sp, velocity_sp, torque_sp;
  int32_t position_est, velocity_est, acceleration_est;
  AxisTrajectory_t Traj;
  const char *axis_name;
  bool currently_homing;
  bool home_reached;

  Stepper &motor;
};
}  // namespace Actuators
#endif /* LINEARAXIS_HPP_ */
