#ifndef OXYGENSENSOR_HPP_
#define OXYGENSENSOR_HPP_

#include "board_configuration.hpp"
namespace Sensors{
struct OxygenSensorParameters{
    int a;
    // ...
};

struct OxygenSensorData{
    int a;
    // ...

};

class OxygenSensor{
public:
OxygenSensor();

void Enable();

void setParameters(OxygenSensorParameters &param);

void getReading(OxygenSensorData &data);

private:

void write_settings();

};
}
#endif // OXYGENSENSOR_HPP_
