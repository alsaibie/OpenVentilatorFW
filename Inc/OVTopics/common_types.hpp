#ifndef COMMON_TYPES_HPP_
#define COMMON_TYPES_HPP_
#include <cstdint>

namespace OVTopics {
struct _msgCore {
  uint32_t tick_stamp { 0 };
  uint32_t msg_count { 0 };
};

}

namespace Actuators {

/* Generic Motion Trajectory */
typedef struct {
  uint32_t *traj_arr;
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

namespace UI {
enum class UserSystem_Modes {
  Manual_Mode = 1,
  Pressure_Control,
  Volume_Control,
  Pressure_Assist,

  EndOfList
};

enum class UserInputSource {
  Onboard = 1,
  Offboard
};

inline UserSystem_Modes& operator++(UserSystem_Modes &m) {
  m = static_cast<UserSystem_Modes>(static_cast<int>(m) + 1);

  if (m == UserSystem_Modes::EndOfList) {
    m = UserSystem_Modes::Manual_Mode;
  }

  return m;
}

}

#endif
