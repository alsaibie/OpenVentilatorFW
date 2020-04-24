#include "board_configuration.hpp"
#include "OpenVentilator/OpenVentilator.hpp"
#include "OVTopics/operation_status.hpp"
#include "OVTopics/system_status.hpp"
#include "OVTopics/user_input.hpp"
#include "UI/Button.hpp"
#include "UI/Potentiometer.hpp"

#include <cstring>
using namespace UI;
using namespace OVRTOS;
using namespace OVTopics;


// TODO: Map button functions for each operation mode and display interface:
class UIManager : public OVThread {
   public:
    UIManager()
        : OVThread("UI Manager", 128, ui_m_priority, 200),
          btn1("Button 1", BTN1_Pin, &on_button_1_press),
          btn2("Button 2", BTN2_Pin, &on_button_2_press),
          btn3("Button 3", BTN3_Pin, &on_button_3_press),
          pot1("Potentiometer 1", POT1_ADCPin),
          pot2("Potentiometer 2", POT2_ADCPin),
          pot3("Potentiometer 3", POT3_ADCPin),
          user_input_pub(gUserInputOVQHandle),
          system_status_sub(gSystemStatusOVQHandle, std::bind(&UIManager::on_system_status_read, this,
                                                              std::placeholders::_1)),
          operation_status_sub(gOperationStatusOVQHandle, std::bind(&UIManager::on_operation_status_read, this,
                                                                    std::placeholders::_1)){
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
        uint32_t last_millis = HAL_GetTick();
        while (1) {
            /* Spin Sampling */
            pot1.spinSampler(HAL_GetTick() - last_millis);
            pot2.spinSampler(HAL_GetTick() - last_millis);
            pot3.spinSampler(HAL_GetTick() - last_millis);

            last_millis = HAL_GetTick();

            /* Read Inputs */
            auto pot1val = pot1.getReading();
            auto pot2val = pot2.getReading();
            auto pot3val = pot3.getReading();

            char arr[10];
            itoa(pot2val, arr, 10);
            /* Translated to Commands */


            //TODO: a button press changes

            userInputMsg.system_mode = user_system_mode;

            /* Pot Inputs */
            uint32_t adc_range = (2 ^ 12);

            userInputMsg.flow_sp_lpm =
                pot1val * (params.flow_rate_lpm_max - params.flow_rate_lpm_min) / adc_range + params.flow_rate_lpm_min;
            userInputMsg.rate_sp_hz =
                pot2val * (params.rate_hz_max - params.rate_hz_min) / adc_range + params.rate_hz_min;
            userInputMsg.IE_ratio = pot3val * (params.ie_max - params.ie_max) / adc_range + params.ie_min;


            /* Publish Commands */
            user_input_pub.publish(userInputMsg);

            /* Read Status & Display Updates in cb */
            system_status_sub.receive();

            operation_status_sub.receive();

            //TODO: REFRESH DISPLAY

            /* Wrap run */
            thread_lap();
        }
    }

   private:
    void process_buttons(){

    	/* Switch to next user mode on long btn 2 press */
    	static uint32_t last_btn2pressed_millis = 0;
    	if(HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == SET){
    		if(HAL_GetTick() - last_btn2pressed_millis > btn2_delay_ms){
    			++user_system_mode;
    		}

    	}else{
    		last_btn2pressed_millis = HAL_GetTick();
    	}

    }

    static void on_button_1_press(void) {}

    static void on_button_2_press(void) {
    	btn2_pressed = true; //TODO: no need for interrupt, just poll instead
    }

    static void on_button_3_press(void) {}

    void on_system_status_read(const OVTopics::SystemStatus_msg_t &msg) {
    	/* Display on monitor */
    }

    void on_operation_status_read(const OVTopics::OperationStatus_msg_t &msg) {
    	/* Display on monitor */
    }

    static UserSystem_Modes user_system_mode;

    Button btn1, btn2, btn3;
    static bool btn2_pressed;
    static uint32_t const btn2_delay_ms = 2000;

    Potentionmeter pot1, pot2, pot3;

    /* Pubs */
    OVQueuePublisher<OVTopics::UserInput_msg_t> user_input_pub;
    OVTopics::UserInput_msg_t userInputMsg;
    /* Subs */
    OVQueueSubscriber<OVTopics::SystemStatus_msg_t> system_status_sub;
    SystemStatus_msg_t systemStatusMsg;
    OVQueueSubscriber<OVTopics::OperationStatus_msg_t> operation_status_sub;
    OperationStatus_msg_t operationStatusMsg;
};


UserSystem_Modes UIManager::user_system_mode = UserSystem_Modes::Manual_Mode;
bool UIManager::btn2_pressed = false;

void start_ui_manager() {
    UIManager *ptr_input = new UIManager();
    ptr_input->start();
}
