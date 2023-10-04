#pragma once

#include <cstdlib>

#include "ZOQ/callback.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;

class oneshot_timer_t {
  public:
    class event_t {
      public:
        constexpr event_t() = default;
        constexpr event_t(callback_t cb, size_t ticks) : _cb(cb), _ticks_left(ticks) {}
        constexpr bool valid() const { return _cb.valid(); };
        constexpr bool expired() const { return (0 == _ticks_left); }
        constexpr void execute() const { _cb.execute(); }
        constexpr void clear() { _cb.clear(); }

        callback_t _cb{};
        size_t _ticks_left = 0;
    };

    oneshot_timer_t(TIM_HandleTypeDef& htim);
    ~oneshot_timer_t();
    void schedule(const callback_t& cb, size_t ticks);

  private:
    TIM_HandleTypeDef& _htim;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
