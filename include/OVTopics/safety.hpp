#ifndef SAFETY_HPP_
#define SAFETY_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    int a;
} Safety_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSafetyOVQHandle;

#endif