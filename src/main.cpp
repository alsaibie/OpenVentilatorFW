#include <Arduino.h>
#include "OpenVentilator/OpenVentilator.hpp"

void setup() {
    //TODO: Reload EEPROM

    Serial.begin(115200);
    StartOpenVentilator();
}

void loop() {/* Nothing to see here. All in rtos threads */}