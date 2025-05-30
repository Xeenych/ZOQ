#pragma once

#include "ZOQ/utils/callback.hpp"

namespace ZOQ::itf {

class it_pin_itf {
  public:
    using callback_t = ZOQ::utils::Callback<>;

    virtual void set_callback(const callback_t& cb) = 0;
    virtual ~it_pin_itf() = default;
};

}  // namespace ZOQ::itf
