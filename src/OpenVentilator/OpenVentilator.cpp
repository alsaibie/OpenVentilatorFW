#include "OpenVentilator/OpenVentilator.hpp"
#include "OVRTOS/OVRTOS.hpp"

/* Include Topics */
#include "OVTopics/operation_modes.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/actuator_commands.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"

std::vector<OVRTOS::OVQueueHandle_t *> OVRTOS::OVQueueBase::ptrOVQH;

/* Instantiate OVQHandles */
OVRTOS::OVQueueHandle_t gOperationModesOVQHandle{NULL, "Operation Modes", 1};
OVRTOS::OVQueueHandle_t gSafetyOVQHandle{NULL, "Safety", 1};
OVRTOS::OVQueueHandle_t gSensorStatusOVQHandle{NULL, "Sensor Readings", 10};
OVRTOS::OVQueueHandle_t gActuatorCommandsOVQHandle{NULL, "Actuator Commands", 10};
OVRTOS::OVQueueHandle_t gSystemStatusOVQHandle{NULL, "System Status", 5};
OVRTOS::OVQueueHandle_t gUserInputOVQHandle{NULL, "User Input", 1};

void StartOpenVentilator() {

    pinMode(LED_BUILTIN, OUTPUT);

    /* Start Threads */
    start_master_manager();
    start_safety_manager();
    start_controller_manager();
    start_sensor_manager();
    start_ui_manager();
    start_actuator_manager();
    start_emulator_manager();

    vTaskStartScheduler();
}