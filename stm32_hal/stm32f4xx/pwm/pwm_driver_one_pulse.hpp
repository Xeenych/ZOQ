#pragma once
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::pwm {
class pwm_driver_one_pulse_t {
    pwm_driver_one_pulse_t(TIM_HandleTypeDef* htim, uint32_t OnePulseMode);
    ~pwm_driver_one_pulse_t();

    pwm_driver_one_pulse_t(pwm_driver_one_pulse_t const&) = delete;
    pwm_driver_one_pulse_t& operator=(pwm_driver_one_pulse_t const&) = delete;

  private:
    TIM_HandleTypeDef* const _htim;
};
}  // namespace ZOQ::stm32_hal::stm32f4xx::pwm
