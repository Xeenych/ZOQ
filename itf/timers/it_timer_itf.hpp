#pragma once

#include "utils/callback.hpp"

namespace ZOQ::itf {

class it_timer_itf {
  public:
    using callback_t = ZOQ::utils::Callback<>;

    virtual void set_callback(const callback_t& cb) = 0;
    virtual ~it_timer_itf() = default;
};

}  // namespace ZOQ::itf