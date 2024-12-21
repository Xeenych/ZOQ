#pragma once

#include <cassert>

#include "stm32f4xx_hal_tim.h"
#include "timer_itf.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

class basic_it_timer_t : public timer_itf {
  public:
    basic_it_timer_t(TIM_HandleTypeDef& htim) : _htim{htim} {
        auto status = HAL_TIM_Base_Start_IT(&_htim);
        assert(HAL_OK == status);
    }

    constexpr void set_callback(const callback_t& cb) override { _callback = cb; }

    void on_interrupt() {
        assert(__HAL_TIM_GET_FLAG(&_htim, TIM_FLAG_UPDATE) == SET);
        __HAL_TIM_CLEAR_IT(&_htim, TIM_IT_UPDATE);
        _callback.execute();
    }

    basic_it_timer_t(const basic_it_timer_t&) = delete;
    basic_it_timer_t& operator=(const basic_it_timer_t&) = delete;

    ~basic_it_timer_t() {
        auto status = HAL_TIM_Base_Stop_IT(&_htim);
        assert(HAL_OK == status);
    }

  private:
    TIM_HandleTypeDef& _htim;
    callback_t _callback{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
