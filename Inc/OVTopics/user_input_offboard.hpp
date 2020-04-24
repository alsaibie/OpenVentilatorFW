#ifndef USER_INPUT_OFFBOARD_HPP_
#define USER_INPUT_OFFBOARD_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "OVTopics/common_types.hpp"
/*
 *
 */
namespace OVTopics {

typedef struct : public _msgCore{
  uint32_t rate_sp_hz { 0 };
  float flow_sp_lpm { 0 };
  uint32_t IE_ratio { 0 };
  UI::UserSystem_Modes system_mode { UI::UserSystem_Modes::Manual_Mode };
} UserInputOffboard_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gUserInputOffboardOVQHandle;

#endif
