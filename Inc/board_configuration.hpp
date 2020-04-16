
#ifndef BOARD_CONFIGURATION_HPP_
#define BOARD_CONFIGURATION_HPP_

struct LED_CONFIG_T{
    int rs_pin;
    int en_pin;
    int data_pins[8];
};

const LED_CONFIG_T LED_CONFIG = {0, 0, {1,2,3,4,5,6,7,8}};

#endif /* BOARD_CONFIGURATION_HPP_ */
