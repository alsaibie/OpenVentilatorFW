#ifndef OVTHREAD_HPP_
#define OVTHREAD_HPP_

#include "FreeRTOS.h"
#include "task.h"

namespace OVRTOS {

class OVThread {
   public:
    inline uint32_t start() {
        // TODO: Add check guards
        if (xTaskCreate(&task, thread_name, thread_stack_depth, (void *)this, thread_priority, &task_handle) ==
            pdPASS) {
            return pdPASS;
        } else {
            return pdFAIL;
        }
    }

    inline char *get_name() { return pcTaskGetName(task_handle); }

   protected:
    OVThread(const char *name, uint32_t stack_depth, uint32_t priority, uint32_t Ts_ms)
        : thread_stack_depth(stack_depth),
          thread_priority(priority),
          t_sampling_ticks(pdMS_TO_TICKS(Ts_ms)),
          initializedDelay(false) {
        thread_name = name;
    };

    inline static void task(void *arg) {
        OVThread *ptr = static_cast<OVThread *>(arg);
        ptr->run();
    }

    virtual void run() { /* Redefine this function when inheriting */

        /* Do thread stuff */

        thread_lap(); /* Call to complete the thread period */
    }

    inline void thread_lap() {
        if (!initializedDelay) {
            initializedDelay = true;
            previousWakeTick = xTaskGetTickCount();
        }

        vTaskDelayUntil(&previousWakeTick, t_sampling_ticks / portTICK_PERIOD_MS);
    }

   private:
    const char *thread_name;

    TaskHandle_t task_handle;
    uint32_t thread_stack_depth;
    uint32_t thread_priority;
    TickType_t t_sampling_ticks;
    bool initializedDelay;
    TickType_t previousWakeTick;
};

}  // namespace OVRTOS

#endif /* OVTHREAD_HPP_ */