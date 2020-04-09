#ifndef ROTATINGAXIS_HPP_
#define ROTATINGAXIS_HPP_

/*TODO: A rotating axis is almost identical to a linear one,
 but this may change as implementation progresses */
#include "Stepper.hpp"
#include "common_types.hpp"
namespace Actuators{
typedef struct {
    uint32_t gear_ratio;      /* [RPS to RPS] */
    uint32_t motion_range;    /* [rad] */
    int32_t lower_limit_pos;  /* [rad] */
    int32_t max_pos; /* [rad] */
    int32_t min_pos; /* [rad] */
    uint32_t max_velocity; /* [rad/s] */
    int32_t max_acc; /* [rad/s^2] */
    int32_t min_acc; /* [rad/s^2] */

} RotatingAxis_config_t;

typedef MotorDirection_t AxisDirection_t;

class RotatingAxis {
    public:
    RotatingAxis(const char *name, RotatingAxis_config_t &cfg, Stepper &mot)
    : config(cfg), motor(mot){
        axis_name = name;
    }
    
    /* Periodic Motor Control Call */
    void spinAxis(uint32_t dt);

    inline void setMode(const Actuator_modes_t &m){ control_mode = m;}
    /* Position Control Mode Cmd*/
    inline void setPosition(const int32_t &p){position = p;}
    /* Speed Control Mode Cmd */
    inline void setVelocity(const int32_t &v){velocity = v;}
    /* Torque Control Mode Cmd*/
    inline void setTorque(const int32_t &tau){torque = tau;}
    /* Position Trajectory Mode Cmd*/
    
    //TODO: resolve update ongoing trajectory: or wait until complete? 
    inline void updatePosTrajectory(const AxisTrajectory_t &ptraj){posTraj = ptraj;}
  
    private:
    Actuator_modes_t control_mode;
    RotatingAxis_config_t config;
    int32_t position, velocity, torque;
    AxisTrajectory_t posTraj, velTraj, torqTraj;
    const char * axis_name;

    Stepper &motor;
};
}
#endif /* LINEARAXIS_HPP_ */