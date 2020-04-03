#ifndef OVQUEUE_HPP_
#define OVQUEUE_HPP_


#include "FreeRTOS.h"
#include "queue.h"
#include <vector>

namespace OVRTOS {

typedef enum { Receive = 0, Peek } OVQReadMode;

typedef struct {
    QueueHandle_t handle = NULL;
    const char *name;
    uint32_t length;
} OVQueueHandle_t;

class OVQueueBase {
   public:
    inline const char *get_name() { return ovqh.name; }

    inline QueueHandle_t get_handle() { return ovqh.handle; }

    static std::vector<OVQueueHandle_t *> ptrOVQH;

   protected:
    OVQueueBase(OVQueueHandle_t &qh, size_t msg_size) : ovqh(qh) {
        if (ovqh.handle == NULL) {
            /* Create Queue if not created */
            ovqh.handle = xQueueCreate(ovqh.length, msg_size);
            ptrOVQH.push_back(&ovqh);
        }
    }

    OVQueueHandle_t &ovqh;
};

template <typename OVQueuemsgT> /* Queue Data Type */
class OVQueuePublisher : public OVQueueBase {
   public:
    OVQueuePublisher(OVQueueHandle_t &qh) : OVQueueBase(qh, sizeof(OVQueuemsgT)) {}

    inline uint32_t publish(OVQueuemsgT &msg) {
        if (xQueueSendToBack(ovqh.handle, &msg, (TickType_t)2) == pdTRUE) {
            return pdPASS;
        } else {
            return pdFAIL;
        }
    }

   private:
    OVQueuemsgT msg;
};

template <typename OVQueuemsgT>
class OVQueueSubscriber : public OVQueueBase {
    typedef void (*callbackT)(const OVQueuemsgT &);

   public:
    OVQueueSubscriber(OVQueueHandle_t &qh, callbackT cb_, OVQReadMode mode)
        : OVQueueBase(qh, sizeof(OVQueuemsgT)), cb(cb_), read_mode(mode) {}

    inline uint32_t receive() {
        if (read_mode == Receive) {
            if (xQueueReceive(ovqh.handle, &msg, (TickType_t)10) == pdPASS) {
                cb(msg);
                return pdPASS;
            } else {
                return pdFAIL;
            }
        } else if (read_mode == Peek) {
            if (xQueuePeek(ovqh.handle, &msg, (TickType_t)10) == pdPASS) {
                cb(msg);
                return pdPASS;
            } else {
                return pdFAIL;
            }
        } else {
            return pdFAIL;
        }
    }

   private:
    OVQueuemsgT msg;
    callbackT cb;
    OVQReadMode read_mode;
};

}  // namespace OVRTOS

#endif