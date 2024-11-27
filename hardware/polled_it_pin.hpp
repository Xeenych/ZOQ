#pragma once

#include "drv/itf/i_pin_it_itf.hpp"
#include "drv/itf/i_pin_itf.hpp"

namespace common {

// Ножка, которая генерирует прерывание по таймеру
class polled_it_pin_t : public itf::i_pin_it_itf {
  public:
    polled_it_pin_t(itf::i_pin_itf& pin) : _pin{pin}, _last_state{get()} {}

    polled_it_pin_t(const polled_it_pin_t&) = delete;
    polled_it_pin_t& operator=(const polled_it_pin_t&) = delete;

    bool get() override { return _pin.get(); }
    void set_callback(callback_t cb, void* ctx) override
    {
        _cb = cb;
        _ctx = ctx;
    }

    void poll()
    {
        auto new_state = get();
        // Falling edge interrupt
        if (true == _last_state && false == new_state) {
            if (_cb)
                _cb(_ctx);
        }
        _last_state = new_state;
    }

  private:
    itf::i_pin_itf& _pin;
    callback_t _cb = nullptr;
    void* _ctx = nullptr;

    bool _last_state;
};

}  // namespace common
