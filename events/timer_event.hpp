#pragma once

#include <cstdint>

#include "ZOQ/callback.hpp"
#include "ZOQ/events/event.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;

class oneshot_event_t {
  public:
    oneshot_event_t(scheduler_itf& s, const callback_t& cb) : _e{s, cb} {}
    void arm(uint32_t period) { _e.arm(period, 0); };
    void disarm() { _e.disarm(); };
    bool armed() const { return _e.armed(); }

  private:
    event_t _e;
};

class periodic_event_t {
  public:
    periodic_event_t(scheduler_itf& s, const callback_t& cb) : _e{s, cb} {}
    void arm(uint32_t period) { _e.arm(0, period); }
    void disarm() { _e.disarm(); }
    bool armed() const { return _e.armed(); }

  private:
    event_t _e;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers