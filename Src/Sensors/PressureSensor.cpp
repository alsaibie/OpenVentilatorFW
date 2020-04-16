#include "Sensors/PressureSensor.hpp"

using namespace Sensors;


void PressureSensor::spinSampler(uint32_t dt){
    /* Implement a sensor filter and convert the analog reading to 
     * pressure reading
     *  */

    auto adc_v_read = getSample() * parameters.sensitivity - parameters.offset;
    
    /* Filter */
    pressure_data.voltage = adc_v_read; // TODO: Filter

    /* Convert */
    pressure_data.P_mmH2O = pressure_data.voltage * parameters.v_to_mmH2O;
}
