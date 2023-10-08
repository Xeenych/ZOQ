#pragma once

#include "event_loopT.hpp"

namespace ZOQ::async {

template <typename T, size_t SIZE>
class active_object_t {
  public:
  protected:
    event_loop_T<T, SIZE> _loop;
};

}  // namespace ZOQ::async
