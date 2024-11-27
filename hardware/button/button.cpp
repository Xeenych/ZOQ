#include "button.hpp"

static peripherals::button_t* _instance = nullptr;

namespace peripherals {

button_t::button_t(i_pin_it_itf& pin, size_t debounce_ticks) : _pin(pin), _debounce_ticks(debounce_ticks)
{
    pin.set_callback(+[](void* c) { static_cast<button_t*>(c)->pin_handler(); }, this);
}

void button_t::pin_debounce()
{
    auto new_state = _pin.get();

    if (_cb)
        _cb(_ctx, new_state ? event_t::released : event_t::pressed);
}

// activated by rising/falling edge interrupt
void button_t::pin_handler()
{
    pin_debounce_evt.arm(_debounce_ticks, 0);
}

}  // namespace peripherals
