#ifndef OPENVENTILATOR_HPP_
#define OPENVENTILATOR_HPP_

#include <Arduino.h>
#include "OVRTOS/OVRTOS.hpp"

/* Main App  - Call in main */
void StartOpenVentilator(void);

/* Declare Global Thread Start C-Compatible Functions - for portability */

void start_master_manager(void);
const RTOSPriority_T master_m_priority = rtosPriorityNormal;

void start_safety_manager(void);
const RTOSPriority_T safety_m_priority = rtosPriorityRealtime;

void start_controller_manager(void);
const RTOSPriority_T controller_m_priority = rtosPriorityRealtime;

void start_sensor_manager(void);
const RTOSPriority_T sensor_m_priority = rtosPriorityNormal;

void start_actuator_manager(void);
const RTOSPriority_T actuator_m_priority = rtosPriorityRealtime;

void start_ui_manager(void);
const RTOSPriority_T ui_m_priority = rtosPriorityIdle;

void start_emulator_manager(void);
const RTOSPriority_T emulator_m_priority = rtosPriorityIdle;

void start_system_monitor(void);
const RTOSPriority_T system_mon_priority = rtosPriorityHigh;


#endif /* MASTER_MANAGER_HPP_ */