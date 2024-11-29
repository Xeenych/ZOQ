#pragma once

#include <cstdint>

#include "ZOQ/callback.hpp"
#include "oneshot_timer.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;

class oneshot_timer_event_t {
  public:
    constexpr oneshot_timer_event_t(oneshot_timer_t& t, const callback_t& cb) : _t{t}, _cb{cb} {}
    constexpr void start(uint32_t period);
    constexpr void stop();

  private:
    oneshot_timer_t& _t;
    const callback_t _cb;
};

class periodic_timer_event_t {
  public:
    constexpr periodic_timer_event_t(oneshot_timer_t& t, const callback_t& cb) : _t{t}, _cb{cb} {}
    constexpr void start(uint32_t period);
    constexpr void stop();

  private:
    oneshot_timer_t& _t;
    const callback_t _cb;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers