#ifndef USER_INPUT_HPP_
#define USER_INPUT_HPP_
#include "OVRTOS/OVQueue.hpp"

/*
 *
 */

namespace OVTopics {

enum class UserSystem_Modes{
    MANUAL_MODE = 0,
    PRESSURE_CONTROL,
    VOLUME_CONTROL,
    PRESSURE_ASSIST, 

    NUM_OF_USERSYSTEMMODES

} ;

enum class ModeBtn_Modes {
    btnMODE1 = 0,
    btnMODE2,
    btnMODE3,
    btnMODE4,

    NUM_OF_BTNMODES
    // TODO: Rename the modes to something meaningful
};

typedef struct {
    uint32_t rate_sp_hz;
    uint32_t flow_sp_lpm;
    uint32_t IE_ratio;
    ModeBtn_Modes btn1_state;
    UserSystem_Modes system_mode;
} UserInput_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gUserInputOVQHandle;

#endif
