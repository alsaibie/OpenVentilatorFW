#ifndef LINEARAXIS_HPP_
#define LINEARAXIS_HPP_

#include "Stepper.hpp"

typedef struct {
    uint32_t gear_ratio;      /* [RPS to mm/s] */
    uint32_t motion_range;    /* [mm] */
    int32_t lower_limit_pos;  /* [mm] */
    int32_t higher_limit_pos; /* [mm] */
    int32_t max_acceleration_fwd;
    int32_t max_acceleration_rev; 
} LinearAxisConfig_t;

typedef struct{
    uint8_t * traj_arr;
    uint32_t traj_arr_length;
    uint32_t sample_time;
    bool cyclical;
}AxisTrajectory_t;

typedef enum{
    Position = 0, 
    Velocity, 
    Torque,
    PositionTrajectory, 
    VelocityTrajectory, 
    TorqueTrajectory,
    
    Num_of_Modes
}LinearAxisControlModes_t;

typedef MotorDirection_t AxisDirection_t;

class LinearAxis {
    public:
    LinearAxis(const char *name, LinearAxisConfig_t &cfg): config(cfg){
        axis_name = name;
    }

    /* Periodic Motor Control Call */
    void spinAxis(uint32_t dt);

    inline void setMode(LinearAxisControlModes_t &m){ control_mode = m;}
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
    LinearAxisControlModes_t control_mode;
    LinearAxisConfig_t config;
    int32_t position, velocity, torque;
    AxisTrajectory_t posTraj, velTraj, torqTraj;
    const char * axis_name;
};

#endif /* LINEARAXIS_HPP_ */