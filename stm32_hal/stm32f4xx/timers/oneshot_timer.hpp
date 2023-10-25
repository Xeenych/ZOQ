#pragma once

#include <cstdlib>

#include "ZOQ/callback.hpp"
#include "stm32f4xx_hal.h"
#include "ZOQ/itf/timers/oneshot_timer_itf.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;
using namespace ZOQ::itf;

class oneshot_timer_t final : public oneshot_timer_itf {
  public:
    oneshot_timer_t(TIM_HandleTypeDef& htim);
    ~oneshot_timer_t() override;
    void schedule(const callback_t& cb, size_t ticks) override;
    static void on_timer_interrupt();

  private:
    TIM_HandleTypeDef& _htim;
    static inline event_t event_table[10];
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
