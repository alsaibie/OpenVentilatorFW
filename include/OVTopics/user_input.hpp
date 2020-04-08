#ifndef USER_INPUT_HPP_
#define USER_INPUT_HPP_
#include "OVRTOS/OVQueue.hpp"

/*
 *
 */

namespace OVTopics {

typedef enum {
    MANUAL_MODE = 0,
    PRESSURE_CONTROL,
    VOLUME_CONTROL,
    PRESSURE_ASSIST, 

    NUM_OF_USERSYSTEMMODES

} UserSystem_modes_t;

typedef enum {
    btnMODE1 = 0,
    btnMODE2,
    btnMODE3,
    btnMODE4,

    NUM_OF_BTNMODES
    // TODO: Rename the modes to something meaningful
} ModeBtn_modes_t;

typedef struct {
    uint32_t rate_sp_hz;
    uint32_t flow_sp_lpm;
    uint32_t IE_ratio;
    ModeBtn_modes_t btn1_state;
    UserSystem_modes_t system_mode;
} UserInput_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gUserInputOVQHandle;

#endif
