#ifndef SYSTEM_STATUS_HPP_
#define SYSTEM_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {

    struct {
        float exp;
        float average;
        float max;
        float min;
    }P;

    struct {
        float slpm;

    }Q;

    struct {
        float max;
        float min;
        float average;
    }V;

    struct{
        uint32_t uptime;

    }system;
    
} SystemStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSystemStatusOVQHandle;

#endif
