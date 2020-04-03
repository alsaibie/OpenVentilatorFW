#ifndef USER_INPUT_HPP_
#define USER_INPUT_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    int sw;
} UserInput_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gUserInputOVQHandle;

#endif
