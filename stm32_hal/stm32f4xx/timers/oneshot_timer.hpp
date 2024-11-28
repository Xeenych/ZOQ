#pragma once

#include <cassert>

#include "ZOQ/callback.hpp"
#include "ZOQ/itf/timers/oneshot_timer_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ::itf;

class oneshot_timer_t final : public oneshot_timer_itf {
  public:
    oneshot_timer_t(TIM_HandleTypeDef& htim) : _htim{htim}
    {
        auto status = HAL_TIM_Base_Start_IT(&_htim);
        assert(HAL_OK == status);
    }

    ~oneshot_timer_t() override;
    void schedule(const callback_t& cb, size_t ticks) override;
    void on_interrupt();

  private:
    TIM_HandleTypeDef& _htim;
    event_t event_table[10]{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
