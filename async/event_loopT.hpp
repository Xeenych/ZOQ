#pragma once

#include <cassert>
#include <cstdlib>

namespace ZOQ::async {

template <typename T>
using fn_T = void (T::*)();

template <typename T>
struct event_T {
    void execute() const { (_context->*_function)(); }
    void clear() { _function = nullptr; }
    bool valid() const { return nullptr == _function; }
    fn_T<T> _function;
    T* _context;
};

template <typename T, size_t SIZE>
class event_loop_T {
  public:
    void activate()
    {
        for (auto& e : _events) {
            if (e.valid()) {
                auto ee = e;
                e.clear();
                ee.execute();
            }
        }
    }

    void push(event_T<T> new_event)
    {
        for (auto& e : _events)
            if (!e.valid()) {
                e = new_event;
                return;
            }
        assert(false);
    }

  private:
    event_T<T> _events[SIZE];
};
}  // namespace ZOQ::async