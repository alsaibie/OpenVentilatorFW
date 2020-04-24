#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/safety.hpp"
#include "OVTopics/sensor_status.hpp"

#include "Sensors/FlowSensor.hpp"
#include "Sensors/PressureSensor.hpp"

using namespace OVRTOS;
using namespace OVTopics;
using namespace Sensors;

FlowSensorParameters_t fsensor_param = { .sensitivity = 2, .offset = 0,
    .v_to_mmH2O = 2, .d_mmH2O_to_SLPM = 20, .cut_off_f = 20 };
FlowSensor fsensor(fsensor_param);

PressureSensorParameters_t psensor_param = { .sensitivity = 2, .offset = 0,
    .v_to_mmH2O = 2, .cut_off_f = 20 };
PressureSensor psensor(psensor_param);

class SensorManager : public OVThread {
 public:
  SensorManager() :
      OVThread("Sensor Manager", 128, sensor_m_priority, 10),
      sensor_status_pub(gSensorStatusOVQHandle),
      safety_sub(
          gSafetyOVQHandle,
          std::bind(&SensorManager::on_safety_read, this,
                    std::placeholders::_1)),
      pressure_data { 0 },
      flow_data { 0 } {
  }

 protected:
  virtual void run() {
    uint32_t last_millis = HAL_GetTick();
    while (1) {

      fsensor.spinSampler(HAL_GetTick() - last_millis);
      psensor.spinSampler(HAL_GetTick() - last_millis);
      last_millis = HAL_GetTick();

      flow_data = fsensor.getReading();
      pressure_data = psensor.getReading();

      SensorMsg.P_mmH2O = pressure_data.P_mmH2O;
      SensorMsg.Q_SLPM = flow_data.Q_SLPM;

      sensor_status_pub.publish(SensorMsg);
      thread_lap();
    }
  }

 private:
  void on_safety_read(const Safety_msg_t &msg) {
    /* Future placement */
  }

  /* Pubs */
  OVQueuePublisher<SensorStatus_msg_t> sensor_status_pub;
  SensorStatus_msg_t SensorMsg;

  /* Subs */
  OVQueueSubscriber<Safety_msg_t> safety_sub;
  Safety_msg_t safetyMsg;
  /* SensorData */
  FlowSensorData_t flow_data;
  PressureSensorData_t pressure_data;

};

void start_sensor_manager() {
  SensorManager *ptr = new SensorManager();
  ptr->start();
}
