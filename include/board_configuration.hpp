
#ifndef BOARD_CONFIGURATION_HPP_
#define BOARD_CONFIGURATION_HPP_

#define MOTOR_LEFT_STEP_PIN 1
#define MOTOR_LEFT_DIR_PIN 2
#define MOTOR_RIGHT_STEP_PIN 3
#define MOTOR_RIGHT_DIR_PIN 4

#define POT1_PIN PA1
#define POT2_PIN PA4
#define POT3_PIN PB0
#define POT4_PIN PC1

#define BTN1_PIN PH1
#define BTN2_PIN PA0
#define BTN3_PIN PC12
#define BTN4_PIN PC10
#define BTN5_PIN PC13

#define X_LED_PIN
#define Y_LED_PIN
#define Z_LED_PIN


struct LED_CONFIG_T{
    int rs_pin;
    int en_pin;
    int data_pins[8];
};

const LED_CONFIG_T LED_CONFIG = {0, 0, {1,2,3,4,5,6,7,8}};
#define PRESSURE_SENS_ANALOG_PIN PC1

#define FLOW_SENS_ANALOG_PIN PC0

#define oxygen_sensor____

#endif /* BOARD_CONFIGURATION_HPP_ */