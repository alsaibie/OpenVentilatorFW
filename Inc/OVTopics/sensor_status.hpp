#ifndef SENSOR_STATUS_HPP_
#define SENSOR_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct : public _msgCore {
  float Q_SLPM { 0 };
  float P_mmH2O { 0 };
} SensorStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSensorStatusOVQHandle;

#endif
