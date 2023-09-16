#pragma once

#include <cstdlib>

namespace ZOQ::itf {

class delay_timer_itf {
  public:
    // Blocking delay
    virtual void delay_ticks(size_t ticks) = 0;
    virtual ~delay_timer_itf() = default;
};

}  // namespace ZOQ::itf