#pragma once

#include "callback.hpp"

namespace ZOQ::itf {

class it_timer_itf {
  public:
    virtual void set_callback(const callback_t& cb) = 0;
    virtual ~it_timer_itf() = default;
};

}  // namespace ZOQ::itf