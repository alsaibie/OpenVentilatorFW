#ifndef FLOWSENSOR_HPP_
#define FLOWSENSOR_HPP_

#include "board_configuration.hpp"
#include <Arduino.h>

struct FlowSensorParameters{
    int a;
    // ...
};

struct FlowSensorData{
    int a;
    // ...

};

class FlowSensor{
public:
FlowSensor();

void Enable();

void setParameters(FlowSensorParameters &param);

void getReading(FlowSensorData &data);

private:

void write_settings();


};

#endif // FLOWSENSOR_HPP_
