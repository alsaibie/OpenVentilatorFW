#ifndef SYSTEM_STATUS_HPP_
#define SYSTEM_STATUS_HPP_
#include "OVRTOS/OVQueue.hpp"

namespace OVTopics {

typedef struct : public _msgCore {
  struct {
    float exp;
    float average;
    float max;
    float min;
  } P { 0 };

  struct {
    float slpm;

  } Q { 0 };

  struct {
    float max;
    float min;
    float average;
  } V { 0 };

  struct {
    uint32_t uptime { 0 };
  } system;

} SystemStatus_msg_t;

}  // namespace OVTopics

extern OVRTOS::OVQueueHandle_t gSystemStatusOVQHandle;

#endif
