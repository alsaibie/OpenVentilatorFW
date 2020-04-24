#ifndef OPERATION_STATUS_HPP_
#define OPERATION_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "OVTopics/common_types.hpp"

namespace OVTopics {

enum class OperationState_Modes {
  DISARMED = 0,
  READY,
  RUNNING,
  RUNNING_W_WARNING,

  NUM_OPERATION_STATES

};

typedef struct : public _msgCore {
  OperationState_Modes operation_state { OperationState_Modes::DISARMED };
  UI::UserInputSource input_source { UI::UserInputSource::Offboard };
} OperationStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gOperationStatusOVQHandle;

#endif
