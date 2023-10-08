#pragma once

#include "ZOQ/callback.hpp"

namespace ZOQ::async {

class event_loop_t {
  public:
    void activate();
    void push(const callback_t& cb);

  private:
    callback_t _events[10];
};
}  // namespace ZOQ::async