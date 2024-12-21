#pragma once

#include "callback.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

class timer_itf {
  public:
    virtual void set_callback(const callback_t& cb) = 0;
    virtual ~timer_itf() = default;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers