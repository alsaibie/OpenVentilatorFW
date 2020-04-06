#include "Sensors/PressureSensor.hpp"

using namespace Sensors;


void PressureSensor::spinSampler(uint32_t dt){
    /* Implement a sensor filter and convert the analog reading to 
     * pressure reading
     *  */

    auto adc_v_read = getSample() * parameters.sensitivity;
    
    /* Filter */
    pressure_data.voltage = 0; // Filtered

    /* Convert */
    pressure_data.P_mmH2O = pressure_data.voltage * parameters.v_to_mmH2O;
}