#ifndef ACTUATOR_COMMANDS_HPP_
#define ACTUATOR_COMMANDS_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "common_types.hpp"

namespace OVTopics {

typedef struct : public _msgCore {

    struct LinearAxis_ {
        int32_t position{0};
        int32_t speed{0};
        int32_t torque{0};
        Actuators::AxisTrajectory_t trajectory;
        Actuators::Actuator_Modes mode{Actuators::Actuator_Modes::Position};
    } primaryAxis;

} ActuatorCommands_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gActuatorCommandsOVQHandle;

#endif
