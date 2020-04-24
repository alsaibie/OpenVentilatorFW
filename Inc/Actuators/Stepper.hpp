#ifndef STEPPER_HPP_
#define STEPPER_HPP_

#include "stm32f4xx_hal.h"
#include "Device/common.h"
#include "math.h"

namespace Actuators {

typedef struct {
	int32_t acc; // steps/s^2
	int32_t v_cruise; // steps/s
	int32_t v_min_start; // step/s
	int32_t v_min_stop; // step/s can theoretically go to a lower speed with initial w
	uint32_t steps_per_rev;
} StepperConfig_t;

typedef struct {
	GPIOPinCombo_t pwm;
	GPIOPinCombo_t dir;
	GPIOPinCombo_t en;
	GPIOPinCombo_t ms1;
	GPIOPinCombo_t ms2;
	GPIOPinCombo_t ms3;
}StepperPinsConfig_t;

enum class MotionState_t {IDLE = 0, ACCELERATE, CRUISE, DECELERATE};
enum class MotorDirection_t { Reverse = 0, Forward } ;

class Stepper {
   public:
    Stepper(const char *name, StepperConfig_t &m_cfg, StepperPinsConfig_t &p_cfg, TIM_HandleTypeDef &pwm_tim, uint32_t pwm_ch, TIM_OC_InitTypeDef & tim_ch_cfg, uint32_t timer_r)
        : param(m_cfg), pins(p_cfg), pwm_timer(pwm_tim), pwm_channel(pwm_ch), pwm_tim_ch_cfg(tim_ch_cfg), timer_rate(timer_r),
		  step_next(0), steps_togo(0), step_postion(0), velocity_est(0), acceleration_est(0),
		  Tus(-1),  motion_state(MotionState_t::IDLE), running(false){
    }

    inline void updateParameters(StepperConfig_t &par){
    	param = par;
    }

    inline void zeroAbsoluteStep(){ step_postion = 0;}

    inline StepperConfig_t geParameters(){return param;}
    inline void enableMotor() {
    	HAL_GPIO_WritePin(pins.en.GPIOx, pins.en.pin, GPIO_PIN_RESET);
//    	HAL_TIM_MspPostInit(&pwm_timer);
    }
    inline void disableMotor() {
    	HAL_GPIO_WritePin(pins.en.GPIOx, pins.en.pin, GPIO_PIN_SET);
    	HAL_GPIO_DeInit(pins.pwm.GPIOx, pins.pwm.pin);
    }

    void moveSteps(int32_t steps);
    void moveToStepPosition(int32_t step_pos);
    void moveWithVelocity(int32_t vel);

    /* Interrupt Call */
    int32_t pulseUpdate();
    int32_t pulseUpdateSimple();


   private:

    void updateSqwFrequency(uint32_t period_us);
    StepperConfig_t &param;
    StepperPinsConfig_t &pins;
    TIM_HandleTypeDef &pwm_timer;
    uint32_t pwm_channel;
    TIM_OC_InitTypeDef &pwm_tim_ch_cfg;
    uint32_t timer_rate; // = 1E5; //Hz TODO: make function of clock setting


    inline int32_t getStepPosition(){ return step_postion;}
    inline int32_t getEstVelocity(){ return velocity_est;}
    inline int32_t getEstAcceleration(){ return acceleration_est;}

    int32_t step_next, steps_togo;
    int32_t step_postion;
    int32_t velocity_est, acceleration_est;
    int32_t Tus;
    MotionState_t motion_state;

    bool running;
};
}  // namespace Actuators
#endif
