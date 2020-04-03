#ifndef STEPPER_HPP_
#define STEPPER_HPP_

#include <Arduino.h>

typedef struct {
    const uint32_t pin_count{4};
    uint32_t enable_pin;
    uint32_t pin_set[4];
} StepperPinConfig_t;

typedef enum { Reverse = 0, Forward } MotorDirection_t;

class Stepper {
   public:
    Stepper(const char *name, StepperPinConfig_t &p_cfg)
        : pin_config(p_cfg), direction(Forward), last_micros(0), step_time_period(1), number_of_steps(4), step_num(0) {
        for (int k; k < pin_config.pin_count; k++) {
            pinMode(pin_config.pin_set[k], OUTPUT);
        }
        pinMode(pin_config.enable_pin, OUTPUT);
    }

    /* RPM -> time period in micros */
    inline void setSpeed(uint32_t &speed) { step_time_period = 60L * 1000000L / number_of_steps / speed; }

    inline void enableMotor() { digitalWrite(pin_config.enable_pin, HIGH); }
    inline void disableMotor() { digitalWrite(pin_config.enable_pin, LOW); }

    inline void stepMotor(int32_t steps) {
        uint32_t steps_remaining = abs(steps);
        direction = steps > 0 ? Forward : Reverse;
        while (steps_remaining) {
            uint32_t micros_now = micros();
            if (micros_now - last_micros >= step_time_period) {
                last_micros = micros_now;
                if (direction == Forward) {
                    step_num = (step_num == number_of_steps) ? 0 : step_num + 1;
                } else if (direction == Reverse) {
                    step_num = (step_num == 0) ? number_of_steps : step_num - 1;
                }
                doSteps(step_num);
                steps_remaining--;
            }
        }
    }

   private:
    inline void doSteps(uint32_t current_step) { /* Apply step sequence */
        static const uint32_t sequence_set[4]{0b1010, 0b0110, 0b0101, 0b1001};
        uint32_t sequence = sequence_set[current_step];
        for (int k = 0; k < pin_config.pin_count; k++) {
            pinMode(pin_config.pin_set[k], sequence & (0x1 << k));
        }
    }
    StepperPinConfig_t pin_config;
    MotorDirection_t direction;
    time_t last_micros;
    time_t step_time_period;
    uint32_t number_of_steps;
    uint32_t step_num;
};

#endif