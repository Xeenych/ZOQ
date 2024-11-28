#pragma once

#include "ZOQ/callback.hpp"

namespace ZOQ::itf {

class it_pin_itf {
  public:
    virtual void set_callback(const callback_t& cb) = 0;
    virtual ~it_pin_itf() = default;
};

}  // namespace ZOQ::itf
