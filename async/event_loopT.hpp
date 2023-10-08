#pragma once

#include "ZOQ/callback.hpp"

namespace ZOQ::async {

template <typename T>
using fn_T = void (T::*)();

template <typename T>
class event_T {
    fn_T<T> _function;
    T* context;
};

template <typename T>
class event_loop_T {
  public:
    void activate();
    void push(const event_T<T>& cb);

  private:
    event_T<T> _events[10];
};
}  // namespace ZOQ::async