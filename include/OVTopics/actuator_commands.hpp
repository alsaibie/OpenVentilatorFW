#ifndef ACTUATOR_COMMANDS_HPP_
#define ACTUATOR_COMMANDS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    struct LinearAxis_ {
        uint32_t acceleration;
        uint32_t speed;
        uint32_t position;
        uint32_t dir;
    }LeftAxis, RightAxis;
} ActuatorCommands_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gActuatorCommandsOVQHandle;

#endif