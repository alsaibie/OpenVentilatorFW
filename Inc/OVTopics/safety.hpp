#ifndef SAFETY_HPP_
#define SAFETY_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {
enum class SystemWarnings_Modes {
  No_Warning = 0x0,
  Low_Pressure = 0x1,
  High_Pressure = 0x2,
  Low_Flow = 0x4,
  High_Flow = 0x8,
  Inconsistent_Flow = 0x16,
  Low_Current = 0x32,
};

enum class SystemErrors_Modes {
  No_Error = 0x0,
  Flow_Sensor_Error = 0x1,
  Pressure_Sensor_Error = 0x2,
  UI_Error = 0x4,
  Actuator_Error = 0x8
};

typedef struct : public _msgCore {
  bool warning_detected { false };
  SystemWarnings_Modes system_warning { SystemWarnings_Modes::No_Warning };
  bool error_detected { false };
  SystemErrors_Modes system_error { SystemErrors_Modes::No_Error };
  bool open_limit_reached { false };
} Safety_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSafetyOVQHandle;

#endif
