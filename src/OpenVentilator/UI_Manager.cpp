#include "OpenVentilator/OpenVentilator.hpp"
#include "UI/Button.hpp"
#include "UI/Potentiometer.hpp"
#include "board_configuration.hpp"

/* Pub Topics */
#include "OVTopics/user_input.hpp"
/* Sub Topics */
#include "OVTopics/operation_status.hpp"
#include "OVTopics/system_status.hpp"

using namespace UI;
using namespace OVRTOS;
using namespace OVTopics;

// TODO: Map button functions for each operation mode and display interface:
class UIManager : public OVThread {
   public:
    UIManager()
        : OVThread("UI Manager", 128, ui_m_priority, 50),
          btn1("Button 1", BTN1_PIN, &on_button_1_press),
          btn2("Button 2", BTN2_PIN, &on_button_2_press),
          btn3("Button 3", BTN3_PIN, &on_button_3_press),
          btn4("Button 4", BTN4_PIN, &on_button_4_press),
          btn5("Button 5", BTN5_PIN, &on_button_5_press),
          pot1("Potentiometer 1", POT1_PIN),
          pot2("Potentiometer 2", POT2_PIN),
          pot3("Potentiometer 3", POT3_PIN),
          pot4("Potentiometer 4", POT4_PIN),
          user_input_pub(gUserInputOVQHandle),
          system_status_sub(gSystemStatusOVQHandle, &on_system_status_peek, Peek),
          operation_status_sub(gOperationStatusOVQHandle, &on_operation_status_peek, Peek),
          user_system_modes(MANUAL_MODE) {
        params.ie_min = 0.5;
        params.ie_max = 3.0;
        params.flow_rate_lpm_max = 60;
        params.flow_rate_lpm_min = 20;
        params.rate_hz_min = 10;
        params.rate_hz_max = 20;
    }

   protected:
    struct {
        float ie_min;
        float ie_max;
        float flow_rate_lpm_min;
        float flow_rate_lpm_max;
        float rate_hz_min;
        float rate_hz_max;
    } params;

    virtual void run() final {
        time_t last_micros = micros();
        while (1) {
            /* Spin Sampling */
            pot1.spinSampler(micros() - last_micros);
            pot2.spinSampler(micros() - last_micros);
            pot3.spinSampler(micros() - last_micros);
            pot4.spinSampler(micros() - last_micros);
            last_micros = micros();

            /* Read Inputs */
            auto pot1val = pot1.getReading();
            auto pot2val = pot2.getReading();
            auto pot3val = pot3.getReading();
            auto pot4val = pot4.getReading();

            /* Translated to Commands */
            OVTopics::UserInput_msg_t ui_msg;
            ui_msg.system_mode = user_system_modes;
            uint32_t adc_range = (2 ^ 12);

            ui_msg.flow_sp_lpm =
                pot1val * (params.flow_rate_lpm_max - params.flow_rate_lpm_min) / adc_range + params.flow_rate_lpm_min;
            ui_msg.rate_sp_hz =
                pot2val * (params.rate_hz_max - params.rate_hz_min) / adc_range + params.rate_hz_min;
            ui_msg.IE_ratio = pot3val * (params.ie_max - params.ie_max) / adc_range + params.ie_min;

            /* Publish Commands */
            user_input_pub.publish(ui_msg);

            /* Read Status */
            system_status_sub.receive();
            operation_status_sub.receive();

            /* Update Display */

            /* Wrap run */
            thread_lap();
        }
    }

   private:
    static void on_button_1_press(void) {}

    static void on_button_2_press(void) {}

    static void on_button_3_press(void) {}

    static void on_button_4_press(void) {}

    static void on_button_5_press(void) {}

    static void on_system_status_peek(const OVTopics::SystemStatus_msg_t &msg) {}

    static void on_operation_status_peek(const OVTopics::OperationStatus_msg_t &msg) {}

    OVTopics::UserSystem_modes_t user_system_modes;

    Button btn1, btn2, btn3, btn4, btn5;
    Potentionmeter pot1, pot2, pot3, pot4;

    /* Pubs */
    OVQueuePublisher<OVTopics::UserInput_msg_t> user_input_pub;

    /* Subs */
    OVQueueSubscriber<OVTopics::OperationStatus_msg_t> operation_status_sub;
    OVQueueSubscriber<OVTopics::SystemStatus_msg_t> system_status_sub;
};

void start_ui_manager() {
    UIManager *ptr_input = new UIManager();
    ptr_input->start();
}
