#ifndef PRESSURESENSOR_HPP_
#define PRESSURESENSOR_HPP_

#include "board_configuration.hpp"
#include <Arduino.h>

struct PressureSensorParameters{
    int a;
    // ...
};

struct PressureSensorData{
    int a;
    // ...

};

class PressureSensor{
public:
PressureSensor();

void Enable();

void setParameters(PressureSensorParameters &param);

void getReading(PressureSensorData &data);

private:

void write_settings();


};

#endif // PRESSURESENSOR_HPP_
