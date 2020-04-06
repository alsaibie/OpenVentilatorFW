#include "Sensors/FlowSensor.hpp"

using namespace Sensors;

void FlowSensor::spinSampler(uint32_t dt){
    /* Implement a sensor filter and convert the analog reading to 
     * pressure reading
     *  */

    auto adc_v_read = getSample() * parameters.sensitivity;
    
    /* Filter */
    flow_data.voltage = 0; // Filtered

    /* Convert */
    flow_data.dP_mmH2O = flow_data.voltage * parameters.v_to_mmH2O;

    flow_data.Q_SLPM = flow_data.dP_mmH2O * parameters.d_mmH2O_to_SLPM; 

}