#ifndef SYSTEM_STATUS_HPP_
#define SYSTEM_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    int a;
} SystemStatus_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSystemStatusOVQHandle;

#endif
