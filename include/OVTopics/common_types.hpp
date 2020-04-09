#ifndef COMMON_TYPES_HPP_
#define COMMON_TYPES_HPP_
#include <cstdint>

namespace Actuators {

/* Generic Motion Trajectory */
typedef struct {
    uint32_t* traj_arr;
    uint32_t traj_arr_length;
    uint32_t sample_time;
    bool cyclical; /* If cyclical: must be a closed path -> end = start */
} AxisTrajectory_t;

 enum class Actuator_Modes {
    Position = 0,
    Velocity,
    Torque,
    PositionTrajectory,
    VelocityTrajectory,
    TorqueTrajectory,

    Num_of_Modes
};

}  // namespace Actuators

#endif