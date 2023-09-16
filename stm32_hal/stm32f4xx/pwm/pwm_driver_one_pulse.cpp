#include "pwm_driver_one_pulse.hpp"

#include <cassert>

namespace ZOQ::stm32_hal::stm32f4xx::pwm {

pwm_driver_one_pulse_t::pwm_driver_one_pulse_t(TIM_HandleTypeDef* htim, uint32_t OnePulseMode) : _htim(htim)
{
    {
        auto status = HAL_TIM_OnePulse_Init(_htim, OnePulseMode);
        assert(HAL_OK == status);
    }

    /*
    {
        TIM_OnePulse_InitTypeDef config = {.OCMode = TIM_OCMODE_TIMING,
                                           .Pulse = 1024,
                                           .OCPolarity = TIM_OCPOLARITY_HIGH,
                                           .OCNPolarity = TIM_OCNPOLARITY_HIGH,
                                           .OCIdleState = TIM_OCIDLESTATE_SET,
                                           .OCNIdleState = TIM_OCNIDLESTATE_SET,
                                           .ICPolarity = TIM_ICPOLARITY_RISING,
                                           .ICSelection = TIM_ICSELECTION_DIRECTTI,
                                           .ICFilter = 100};
        uint32_t OutputChannel = TIM_CHANNEL_1;
        uint32_t InputChannel = TIM_CHANNEL_1;

        auto status = HAL_TIM_OnePulse_ConfigChannel(_htim, &config, OutputChannel, InputChannel);
        assert(HAL_OK == status);
    }
    */
}

pwm_driver_one_pulse_t::~pwm_driver_one_pulse_t()
{
    auto status = HAL_TIM_OnePulse_DeInit(_htim);
    assert(HAL_OK == status);
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::pwm
