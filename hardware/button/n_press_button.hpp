#pragma once

#include <cstdlib>

#include "periph/button/button.hpp"
#include "scheduler/scheduler.hpp"

namespace common {

using peripherals::button_t;

class n_press_button_t {
  public:
    using press_callback_t = void (*)(void* ctx, size_t n_presses);
    using release_callback_t = void (*)(void* ctx, size_t n_pressesы);
    using idle_callback_t = void (*)(void* ctx);
    using hold_callback_t = void (*)(void* ctx);

    struct callbacks_t {
        // Собитие при нажатии кнопки
        press_callback_t press_callback = nullptr;
        // Собитие при отпускании кнопки
        press_callback_t release_callback = nullptr;
        // Кнопка была отпущена в течение idle_timeout
        idle_callback_t idle_callback = nullptr;
        // Кнопка была зажата в течение
        hold_callback_t hold_callback = nullptr;
    };

    n_press_button_t(button_t& button, size_t idle_timeout, size_t hold_timeout)
        : _button{button}, _idle_timeout{idle_timeout}, _hold_timeout{hold_timeout}
    {
        _button.register_callback(
            +[](void* ctx, button_t::event_t e) { static_cast<n_press_button_t*>(ctx)->on_button_event(e); }, this);
    }

    void register_callbacks(void* ctx, const callbacks_t& cb)
    {
        _ctx = ctx;
        _cb = cb;
    }

    void on_button_event(button_t::event_t e)
    {
        if (button_t::event_t::pressed == e) {
            _press_cnt++;
            _hold_evt.arm(_hold_timeout, 0);

            if (_cb.press_callback)
                _cb.press_callback(_ctx, _press_cnt);
        } else {
            _idle_evt.arm(_idle_timeout, 0);
            _hold_evt.disarm();

            if (_cb.release_callback)
                _cb.release_callback(_ctx, _press_cnt);
        }
    }

    n_press_button_t(const n_press_button_t&) = delete;
    n_press_button_t& operator=(const n_press_button_t&) = delete;

  private:
    button_t& _button;
    const size_t _idle_timeout;
    const size_t _hold_timeout;

    // Таймер для отсчета времени покоя.
    // Кнопка находится в покое, если она отпущена
    scheduler::event_t _idle_evt{+[](void* ctx) { static_cast<n_press_button_t*>(ctx)->on_idle_timeout(); }, this};

    // Таймер для зажатия кнопки
    scheduler::event_t _hold_evt{+[](void* ctx) { static_cast<n_press_button_t*>(ctx)->on_hold(); }, this};

    size_t _press_cnt = 0;

    void* _ctx = nullptr;
    callbacks_t _cb{};

    void on_idle_timeout()
    {
        _press_cnt = 0;
        if (_cb.idle_callback)
            _cb.idle_callback(_ctx);
    }

    void on_hold()
    {
        _press_cnt = 0;
        if (_cb.hold_callback)
            _cb.hold_callback(_ctx);
    }
};

}  // namespace common
