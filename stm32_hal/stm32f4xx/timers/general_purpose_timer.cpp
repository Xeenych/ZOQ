#include "general_purpose_timer.hpp"

#include <cassert>

namespace ZOQ::stm32_hal::stm32f4xx::timers {
general_purpose_timer_t::general_purpose_timer_t(TIM_HandleTypeDef& htim) : _htim(htim)
{
    auto status = HAL_TIM_Base_Start(&_htim);
    assert(HAL_OK == status);
}

general_purpose_timer_t::~general_purpose_timer_t()
{
    auto status = HAL_TIM_Base_Stop(&_htim);
    assert(HAL_OK == status);
}

void general_purpose_timer_t::delay_ticks(size_t ticks)
{
    uint16_t start = _htim.Instance->CNT;
    while (uint16_t(_htim.Instance->CNT - start) < ticks) {
    };
}
}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
