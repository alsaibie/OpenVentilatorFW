#include "ArduinoJson.h"
#include "OpenVentilator/OpenVentilator.hpp"

/* Publish Topics */
#include "OVTopics/user_input_offboard.hpp"

/* Subscribe Topics */
#include "OVTopics/operation_status.hpp"
#include "OVTopics/sensor_status.hpp"
#include "OVTopics/system_status.hpp"

using namespace OVRTOS;
using namespace OVTopics;

class PCCommsManager : public OVThread {
   public:
    PCCommsManager()
        : OVThread("PC Communications", 128, pc_comms_m_priority, 50),
          user_input_offboard_pub(gUserInputOffboardOVQHandle),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek),
          sensor_status_sub(gSensorStatusOVQHandle, &on_sensor_status_peek, Peek),
          system_status_sub(gSystemStatusOVQHandle, &on_system_status_peek, Peek) {}

   protected:
    virtual void run() final {
        while (1) {
            operation_status_sub.receive();
            sensor_status_sub.receive();
            system_status_sub.receive();

            /* Send States to PC */
            jdoc["Test"] = "GPS";
            jdoc["Time"] = millis();

            JsonArray data = jdoc.createNestedArray("data");
            data.add(3.234);
            data.add(234.234);

            serializeJson(jdoc, Serial);
            Serial.println();

            /* Receive Commands from PC */

            /* Relay to OVTopic */

            UserInputOffboard_msg_t ui_ob_msg;

            user_input_offboard_pub.publish(ui_ob_msg);

            thread_lap();
        }
    }

   private:
    static void on_operation_status_peek(const OperationStatus_msg_t &msg) {}
    static void on_sensor_status_peek(const SensorStatus_msg_t &msg) {}
    static void on_system_status_peek(const SystemStatus_msg_t &msg) {}
    /* Pubs */
    OVQueuePublisher<UserInputOffboard_msg_t> user_input_offboard_pub;

    /* Subs */
    OVQueueSubscriber<OperationStatus_msg_t> operation_status_sub;
    OVQueueSubscriber<SensorStatus_msg_t> sensor_status_sub;
    OVQueueSubscriber<SystemStatus_msg_t> system_status_sub;

    StaticJsonDocument<1024> jdoc;
};

void start_pc_comms_manager(void) {
    PCCommsManager *ptr = new PCCommsManager();
    ptr->start();
}
