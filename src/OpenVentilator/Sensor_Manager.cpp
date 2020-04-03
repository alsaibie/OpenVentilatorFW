#include "OpenVentilator/OpenVentilator.hpp"

/* Publish Topics */
#include "OVTopics/sensor_status.hpp"

/* Subscribe Topics */
#include "OVTopics/safety.hpp"

using namespace OVRTOS;

class SensorManager : public OVThread {
   public:
    SensorManager()
        : OVThread("Sensor Manager", 128, sensor_m_priority, 10),
          sensor_status_pub(gSensorStatusOVQHandle),
          safety_sub(gSafetyOVQHandle, &on_safety_peek, Peek) {}

   protected:
    virtual void run() {
        while (1) {
            thread_lap();
        }
    }

   private:
    static void on_safety_peek(const OVTopics::Safety_t &msg) {}

    /* Pubs */
    OVQueuePublisher<OVTopics::SensorStatus_t> sensor_status_pub;
    OVTopics::SensorStatus_t SensorMsg;
    /* Subs */
    OVQueueSubscriber<OVTopics::Safety_t> safety_sub;
};

void start_sensor_manager() {
    SensorManager *ptr = new SensorManager();
    ptr->start();
}