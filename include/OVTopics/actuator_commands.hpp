#ifndef ACTUATOR_COMMANDS_HPP_
#define ACTUATOR_COMMANDS_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "common_types.hpp"

namespace OVTopics {

typedef struct {
    struct LinearAxis_ {
        uint32_t position;
        uint32_t speed;
        uint32_t torque;
        Actuators::Actuator_modes_t mode;
    } primaryAxis, secondaryAxis;

} ActuatorCommands_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gActuatorCommandsOVQHandle;

#endif