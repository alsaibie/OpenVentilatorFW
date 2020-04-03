#ifndef OPERATION_MODES_HPP_
#define OPERATION_MODES_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef enum{
PressureControl = 0, 
VolumeControl, 
BreathAssist,

NumOfOpModes

}OperationModes;

typedef struct {
    int a;
} OperationModes_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gOperationModesOVQHandle;

#endif