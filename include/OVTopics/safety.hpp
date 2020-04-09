#ifndef SAFETY_HPP_
#define SAFETY_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {
enum class SystemWarnings_Modes {
    LOW_PRESSURE = 0x1,
    HIGH_PRESSURE = 0x2,
    LOW_FLOW = 0x4,
    HIGH_FLOW = 0x8,
    INCONSISTENT_FLOW = 0x16,
    LOW_CURRENT = 0x32
} ;

enum class SystemErrors_Modes{
    FLOW_SENSOR_ERROR = 0x1,
    PRESSURE_SENSOR_ERROR = 0x2,
    UI_ERROR = 0x4,
    ACTUATOR_ERROR = 0x8
} ;

typedef struct {
    bool warning_detected;
    SystemWarnings_Modes system_warning;
    bool error_detected;
    SystemErrors_Modes system_error;
    bool open_limit_reached;
} Safety_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSafetyOVQHandle;

#endif