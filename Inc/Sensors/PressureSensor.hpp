#ifndef PRESSURESENSOR_HPP_
#define PRESSURESENSOR_HPP_

#include "AnalogInterface.hpp"
#include "board_configuration.hpp"
#include "main.h"
namespace Sensors {
typedef struct{
    float sensitivity; /* [mV]/bit*/
    float offset;
    float v_to_mmH2O; /* [mmH2O] / [mV]*/
    float cut_off_f;
}PressureSensorParameters_t;

typedef struct {
    float voltage; /* Sensor Filtered Voltage Reading */
    float P_mmH2O; /* Gauge Pressure [mmH2O] */
}PressureSensorData_t;

class PressureSensor : AnalogInterface {
   public:
    PressureSensor(PressureSensorParameters_t &param)
        : AnalogInterface("Pressure Sensor", PSENS_ADCPin), parameters(param) {}

    inline void setParameters(PressureSensorParameters_t &param) { parameters = param; }

    inline PressureSensorData_t getReading(void){return pressure_data;};

    virtual void spinSampler(uint32_t dt);

   private:
    PressureSensorParameters_t parameters;
    PressureSensorData_t pressure_data;
};
}  // namespace Sensors
#endif  // PRESSURESENSOR_HPP_
