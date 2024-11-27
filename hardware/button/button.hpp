/*
    Класс обработчика нажатий кнопки.
    Строится поверх драйвера, который реагирует на RISING+FALLLING прерывания
    Генерирует события:
    - кнопка нажата
    - кнопка отпущена

    При поступлении любого прерывания драйвер:
    - перезапускает таймер на debounce_ticks время
    - по истечение таймера:
        - считывает уровень на ножке
        - вызывает соответсвтующий колбек
*/

#pragma once

#include <cstdlib>

#include "drv/itf/i_pin_it_itf.hpp"
#include "scheduler/scheduler.hpp"

namespace peripherals {
using itf::i_pin_it_itf;
using namespace scheduler;

class button_t {
  public:
    enum class event_t { pressed, released };
    using callback_t = void (*)(void* ctx, event_t e);

    // Pin must be configured to rising/falling edge trigger
    // debounce_ticks - время, в тчение которого драйвер ждет установления сигнала
    button_t(i_pin_it_itf& pin, size_t debounce_ticks);
    void register_callback(const callback_t& cb, void* ctx)
    {
        _cb = cb;
        _ctx = ctx;
    }

    button_t(const button_t&) = delete;
    button_t& operator=(const button_t&) = delete;

  private:
    i_pin_it_itf& _pin;
    const size_t _debounce_ticks;
    callback_t _cb = nullptr;
    void* _ctx = nullptr;

    void pin_debounce();
    void pin_handler();

    scheduler::event_t pin_debounce_evt{+[](void* ctx) { static_cast<button_t*>(ctx)->pin_debounce(); }, this};
};

}  // namespace peripherals
