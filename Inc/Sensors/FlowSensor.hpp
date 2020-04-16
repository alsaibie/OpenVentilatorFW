#ifndef FLOWSENSOR_HPP_
#define FLOWSENSOR_HPP_

#include "AnalogInterface.hpp"
#include "board_configuration.hpp"
#include "main.h"
namespace Sensors {
typedef struct  {
    float sensitivity;     /* [mV]/bit*/
    float offset;
    float v_to_mmH2O;      /* [mmH2O] / [mV]*/
    /* Assuming a calibrated spirometer */
    float d_mmH2O_to_SLPM; /* [SLPM]/[mmH2O] */ 
    float cut_off_f;
}FlowSensorParameters_t;

typedef struct  {
    float voltage;  /* Sensor Filtered Voltage Reading */
    float dP_mmH2O; /* Differential Pressure [mmH2O] */
    float Q_SLPM;   /* Flow Rate [Standard Liter per Minute]*/
}FlowSensorData_t;

class FlowSensor : AnalogInterface {
   public:
    FlowSensor(FlowSensorParameters_t &param) 
    : AnalogInterface("Flow Sensor", FSENS_ADCPin), parameters(param) {}

    inline void setParameters(FlowSensorParameters_t &param) { parameters = param; }

    inline FlowSensorData_t getReading(void){ return flow_data;};

    virtual void spinSampler(uint32_t dt);

   private:
    FlowSensorParameters_t parameters;
    FlowSensorData_t flow_data;
};
}  // namespace Sensors

#endif  // FLOWSENSOR_HPP_
