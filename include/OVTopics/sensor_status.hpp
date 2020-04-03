#ifndef SENSOR_STATUS_HPP_
#define SENSOR_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    int p;
} SensorStatus_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSensorStatusOVQHandle;

#endif