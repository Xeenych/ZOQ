#pragma once

#include <cstdlib>

#include "ZOQ/callback.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;

class oneshot_timer_t {
  public:
    struct event_t {
        callback_t* _cb;
        size_t _ticks;
    };

    oneshot_timer_t(TIM_HandleTypeDef& htim);
    ~oneshot_timer_t();
    void register_event(callback_t* cb, size_t ticks);

  private:
    TIM_HandleTypeDef& _htim;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
