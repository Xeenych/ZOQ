#include "superblinker.hpp"

namespace common {

void superblinker_t::after_after_address()
{
    if (_cb)
        _cb(_ctx);
}

void superblinker_t::after_address()
{
    auto lambda = +[](void* c) { static_cast<superblinker_t*>(c)->after_after_address(); };
    _red.blink(n_, {.FlashDurationMs = 100, .PauseDurationMs = 400, .TailDurationMs = 400}, false, lambda, this);
}

void superblinker_t::blink_address(size_t n)
{
    n_ = n;
    auto lambda = +[](void* c) { static_cast<superblinker_t*>(c)->after_address(); };
    _blue.blink(2, {.FlashDurationMs = 100, .PauseDurationMs = 400, .TailDurationMs = 0}, false, lambda, this);
}

void superblinker_t::blink_press()
{
    _red.blink(1, {.FlashDurationMs = 100, .PauseDurationMs = 100, .TailDurationMs = 0}, true, nullptr, nullptr);
}

void superblinker_t::blink_activity()
{
    _blue.blink(1, {.FlashDurationMs = 20, .PauseDurationMs = 20, .TailDurationMs = 0}, false);
}

void superblinker_t::acitivty_off()
{
    _blue.off();
}

void superblinker_t::address_off()
{
    _red.off();
}

}  // namespace common
