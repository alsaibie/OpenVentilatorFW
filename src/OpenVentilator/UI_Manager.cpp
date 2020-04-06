#include "OpenVentilator/OpenVentilator.hpp"
#include "UI/Button.hpp"
#include "UI/Potentiometer.hpp"
#include "board_configuration.hpp"

/* Pub Topics */
#include "OVTopics/user_input.hpp"
/* Sub Topics */
#include "OVTopics/operation_modes.hpp"
#include "OVTopics/system_status.hpp"

using namespace UI;
using namespace OVRTOS;

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
          operation_modes_sub(gOperationModesOVQHandle, &on_operation_modes_peek, Peek) {}

   protected:
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
            OVTopics::UserInput_t userInputMsg;

            userInputMsg.sw = 1;

            /* Publish Commands */
            user_input_pub.publish(userInputMsg);

            /* Read Status */
            system_status_sub.receive();
            operation_modes_sub.receive();

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

    static void on_system_status_peek(const OVTopics::SystemStatus_t &msg) {}

    static void on_operation_modes_peek(const OVTopics::OperationModes_t &msg) {}

    Button btn1, btn2, btn3, btn4, btn5;
    Potentionmeter pot1, pot2, pot3, pot4;

    /* Pubs */
    OVQueuePublisher<OVTopics::UserInput_t> user_input_pub;

    /* Subs */
    OVQueueSubscriber<OVTopics::OperationModes_t> operation_modes_sub;
    OVQueueSubscriber<OVTopics::SystemStatus_t> system_status_sub;
};

void start_ui_manager() {
    UIManager *ptr_input = new UIManager();
    ptr_input->start();
}
