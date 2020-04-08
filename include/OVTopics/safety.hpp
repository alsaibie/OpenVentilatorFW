#ifndef SAFETY_HPP_
#define SAFETY_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef enum {
    LOW_PRESSURE = 0x1,
    HIGH_PRESSURE = 0x2,
    LOW_FLOW = 0x4,
    HIGH_FLOW = 0x8,
    INCONSISTENT_FLOW = 0x16,
    LOW_CURRENT = 0x32
} SystemWarnings_modes_t;

typedef enum {
    FLOW_SENSOR_ERROR = 0x1,
    PRESSURE_SENSOR_ERROR = 0x2,
    UI_ERROR = 0x4,
    ACTUATOR_ERROR = 0x8
} SystemErrors_modes_t;

typedef struct {
    bool warning_detected;
    SystemWarnings_modes_t warning;
    bool error_detected;
    SystemErrors_modes_t error;
    bool open_limit_reached;
} Safety_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSafetyOVQHandle;

#endif