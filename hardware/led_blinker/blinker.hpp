#pragma once

#include <cstdlib>

#include "drv/itf/o_pin_itf.hpp"
#include "scheduler/scheduler.hpp"

namespace common {
using namespace itf;
using namespace scheduler;

class blinker_t {
  public:
    struct config_t {
        uint16_t FlashDurationMs = 300;
        uint16_t PauseDurationMs = 300;
        uint16_t TailDurationMs = 2000;
    };

    using cb_t = void (*)(void* ctx);

    blinker_t(o_pin_itf& led) : _l{led} {}
    ~blinker_t();
    void blink(size_t ntimes, config_t config, bool restart, cb_t on_end = nullptr, void* ctx = nullptr);
    void off()
    {
        _timer_off.disarm();
        _timer_on.disarm();
        _timer_periodic.disarm();
        _l.reset();
    }

    void on() { _l.set(); }

    blinker_t(const blinker_t&) = delete;
    blinker_t& operator=(const blinker_t&) = delete;

  private:
    o_pin_itf& _l;
    event_t _timer_on{+[](void* ctx) { static_cast<blinker_t*>(ctx)->on_timer_off(); }, this};
    event_t _timer_off{+[](void* ctx) { static_cast<blinker_t*>(ctx)->on_timer_on(); }, this};
    event_t _timer_periodic{+[](void* ctx) { static_cast<blinker_t*>(ctx)->period_start(); }, this};

    size_t ntimes_top_ = 0;
    size_t ntimes_ = 0;
    config_t _config{};
    cb_t _cb = nullptr;
    void* _ctx = {};

    void period_start();
    void on_timer_on();
    void on_timer_off();
};

}  // namespace common
