#include "blinker.hpp"

namespace common {

blinker_t::~blinker_t()
{
    off();
}

void blinker_t::blink(size_t ntimes, config_t config, bool restart, cb_t on_end, void* ctx)
{
    if (_timer_periodic.armed() && !restart)
        return;
    off();
    _timer_periodic.disarm();
    ntimes_top_ = ntimes;
    ntimes_ = ntimes;
    _config = config;
    _cb = on_end;
    _ctx = ctx;

    size_t interval = ntimes * (_config.FlashDurationMs + _config.PauseDurationMs) + _config.TailDurationMs;
    _timer_periodic.arm(1, interval);
}

void blinker_t::period_start()
{
    ntimes_ = ntimes_top_;
    on_timer_on();
}

void blinker_t::on_timer_on()
{
    if (0 == ntimes_) {
        off();
        _cb ? _cb(_ctx) : void();
        return;
    }
    ntimes_--;

    on();
    // turn off after kFlashDurationMs
    _timer_on.arm(_config.FlashDurationMs, _config.FlashDurationMs);
    // turn on after kFlashDurationMs + kPauseDurationMs
    _timer_off.arm(_config.PauseDurationMs + _config.FlashDurationMs,
                   _config.PauseDurationMs + _config.FlashDurationMs);
}

void blinker_t::on_timer_off()
{
    _l.reset();
}

}  // namespace common