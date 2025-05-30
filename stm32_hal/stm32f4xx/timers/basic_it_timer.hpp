#pragma once

#include <cassert>

#include "ZOQ/itf/timers/it_timer_itf.hpp"
#include "stm32f4xx_hal_tim.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ::itf;

class basic_it_timer_t : public it_timer_itf {
  public:
    explicit basic_it_timer_t(TIM_HandleTypeDef& htim) : _htim{htim} {
        auto status = HAL_TIM_Base_Start_IT(&_htim);
        assert(HAL_OK == status);
    }

    constexpr void set_callback(const callback_t& cb) override { _callback = cb; }

    void on_interrupt() { _callback.Execute(); }

    basic_it_timer_t(const basic_it_timer_t&) = delete;
    basic_it_timer_t& operator=(const basic_it_timer_t&) = delete;

    ~basic_it_timer_t() override {
        auto status = HAL_TIM_Base_Stop_IT(&_htim);
        assert(HAL_OK == status);
    }

  private:
    TIM_HandleTypeDef& _htim;
    callback_t _callback{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
