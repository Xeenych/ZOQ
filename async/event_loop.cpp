#include "event_loop.hpp"

#include <cassert>

namespace ZOQ::async {

void event_loop_t::activate()
{
    for (auto& e : _events) {
        if (e.valid()) {
            auto ee = e;
            e.clear();
            ee.execute();
        }
    }
}

void event_loop_t::push(const callback_t& cb)
{
    for (auto& e : _events)
        if (!e.valid()) {
            e = cb;
            return;
        }
    assert(false);
}

}  // namespace ZOQ::async
