#ifndef OPERATION_STATUS_HPP_
#define OPERATION_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef enum{
PressureControl = 0, 
VolumeControl, 
BreathAssist,

NumOfOpModes

}Operation_modes_t;

typedef struct {
    int a;
} OperationStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gOperationStatusOVQHandle;

#endif