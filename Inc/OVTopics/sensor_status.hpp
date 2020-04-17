#ifndef SENSOR_STATUS_HPP_
#define SENSOR_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct {
    float Q_SLPM;
    float P_mmH2O;
} SensorStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSensorStatusOVQHandle;

#endif
