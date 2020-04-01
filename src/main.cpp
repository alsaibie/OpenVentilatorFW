#include <Arduino.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

xQueueHandle testQueuehandle;
xTaskHandle testTaskHandle;

void start_task(void *arg) {
    while (1) {
        int t = 1;

        digitalToggle(LED_BUILTIN);
        delay(1000);
    }
}

void setup() {
    
    pinMode(LED_BUILTIN, OUTPUT);
    xTaskCreate(start_task, "Default Task", 128, NULL, 1, &testTaskHandle);
    vTaskStartScheduler();
}

void loop() {}