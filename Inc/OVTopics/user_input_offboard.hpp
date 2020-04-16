#ifndef USER_INPUT_OFFBOARD_HPP_
#define USER_INPUT_OFFBOARD_HPP_
#include "OVRTOS/OVQueue.hpp"
#include "OVTopics/common_types.hpp"
/*
 *
 */
namespace OVTopics {

typedef struct {
    uint32_t rate_sp_hz;
    uint32_t flow_sp_lpm;
    uint32_t IE_ratio;
    UI::UserSystem_Modes system_mode;
} UserInputOffboard_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gUserInputOffboardOVQHandle;

#endif
