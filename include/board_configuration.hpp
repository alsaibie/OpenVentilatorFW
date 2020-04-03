
#ifndef BOARD_CONFIGURATION_HPP_
#define BOARD_CONFIGURATION_HPP_

#define MOTOR_LEFT_STEP_PIN 1
#define MOTOR_LEFT_DIR_PIN 2
#define MOTOR_RIGHT_STEP_PIN 3
#define MOTOR_RIGHT_DIR_PIN 4

#define X_POT_PIN 
#define Y_POT_PIN
#define Z_POT_PIN

#define X_BTN_PIN
#define Y_BTN_PIN
#define Z_BTN_PIN

#define X_LED_PIN
#define Y_LED_PIN
#define Z_LED_PIN


struct LED_CONFIG_T{
    int rs_pin;
    int en_pin;
    int data_pins[8];
};

const LED_CONFIG_T LED_CONFIG = {0, 0, {1,2,3,4,5,6,7,8}};
#define pressure_sensor____

#define flow_sensor____

#define oxygen_sensor____

#endif /* BOARD_CONFIGURATION_HPP_ */