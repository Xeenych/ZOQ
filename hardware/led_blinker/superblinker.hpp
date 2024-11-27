#pragma once
#include "blinker.hpp"
#include "drv/itf/o_pin_itf.hpp"
#include "scheduler/scheduler.hpp"

namespace common {
using namespace scheduler;

class superblinker_t {
  public:
    using cb_t = void (*)(void*);
    superblinker_t(o_pin_itf& led_r, o_pin_itf& led_b) : _red{led_r}, _blue{led_b} {}
    superblinker_t(const superblinker_t&) = delete;
    superblinker_t& operator=(superblinker_t&) = delete;

    void set_callback(cb_t cb, void* ctx)
    {
        _cb = cb;
        _ctx = ctx;
    }

    void blink_address(size_t n);
    void address_off();
    void blink_press();

    void blink_activity();
    void acitivty_off();

  private:
    blinker_t _red;
    blinker_t _blue;

    cb_t _cb = nullptr;
    void* _ctx = nullptr;

    uint32_t n_ = 0;
    void after_address();
    void after_after_address();
};

}  // namespace common
