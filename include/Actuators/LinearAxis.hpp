#ifndef LINEARAXIS_HPP_
#define LINEARAXIS_HPP_

#include "Stepper.hpp"
#include "OVTopics/common_types.hpp"
namespace Actuators{
typedef struct {
    uint32_t gear_ratio;      /* [RPS to mm/s] */
    uint32_t motion_range;    /* [mm] */
    int32_t lower_limit_pos;  /* [mm] */
    int32_t max_pos_fwd; /* [mm] */
    int32_t max_pos_rev; /* [mm] */
    uint32_t max_velocity; /* [mm/s] */
    int32_t max_acc_fwd; /* [mm/s^2] */
    int32_t max_acc_rev; /* [mm/s^2] */

} LinearAxis_config_t;


typedef MotorDirection_t AxisDirection_t;

class LinearAxis {
    public:
    LinearAxis(const char *name, LinearAxis_config_t &cfg, Stepper &mot)
    : config(cfg), motor(mot){
        axis_name = name;
    }
    
    /* Periodic Motor Control Call */
    void spinAxis(uint32_t dt);

    inline void setMode(Actuator_Modes &m){ control_mode = m;}
    /* Position Control Mode Cmd*/
    inline void setPosition(int32_t &p){position = p;}
    /* Speed Control Mode Cmd */
    inline void setVelocity(int32_t &v){velocity = v;}
    /* Torque Control Mode Cmd*/
    inline void setTorque(int32_t &tau){torque = tau;}
    /* Position Trajectory Mode Cmd*/
    
    //TODO: resolve update ongoing trajectory: or wait until complete? 
    inline void updatePosTrajectory(AxisTrajectory_t &ptraj){posTraj = ptraj;}
  
    private:
    Actuator_Modes control_mode;
    LinearAxis_config_t config;
    int32_t position, velocity, torque;
    AxisTrajectory_t posTraj, velTraj, torqTraj;
    const char * axis_name;

    Stepper &motor;
};
}
#endif /* LINEARAXIS_HPP_ */