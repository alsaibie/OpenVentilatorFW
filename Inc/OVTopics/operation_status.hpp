#ifndef OPERATION_STATUS_HPP_
#define OPERATION_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "OVTopics/common_types.hpp"

namespace OVTopics {

enum class OperationState_Modes {
  Disarmed = 0,
  Ready,
  Running,
  Running_Warning,
  Calibrating,
  Num_Operating_States
};


typedef struct : public _msgCore {
  OperationState_Modes operation_state { OperationState_Modes::Disarmed };
  UI::UserInputSource input_source { UI::UserInputSource::Offboard };
  UI::UserSystem_Modes operation_mode {UI::UserSystem_Modes::Manual_Mode};
} OperationStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gOperationStatusOVQHandle;

#endif
