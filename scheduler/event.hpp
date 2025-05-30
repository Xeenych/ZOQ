#pragma once

#include <atomic>

#include "ZOQ/utils/callback.hpp"
#include "ZOQ/scheduler/scheduler_itf.hpp"

namespace ZOQ {

using namespace ZOQ::itf;

class event_t {
    using callback_t = ZOQ::utils::Callback<>;

  public:
    constexpr event_t(scheduler_itf& s, const callback_t& cb) : _s{s}, _cb{cb} { s.add(this); }
    ~event_t();
    // Запускает отложенное событие
    // ctr - число тиков через которое выполнится отложенное событие
    // interval - интервал для перезагрузки таймера
    // arm(100, 0) - однокртаное выполнение события через 100 тиков
    // arm(100, 500) - выполнить событие черз 100 тиков, а потом выполнять каждые 500 тиков
    void arm(uint32_t ctr, uint32_t interval);
    void disarm();
    bool armed() const { return (0 != _ctr); }

    // call this on SysTick interrupt
    void tick();

    [[nodiscard]] bool is_expiring() const { return _expiring; }
    void reload() {
        _ctr = _interval;
        _expiring = false;
    }
    void execute() const { _cb.Execute(); }

    event_t* next() const { return _next; }
    void set_next(event_t* e) { _next = e; }

    event_t(const event_t&) = delete;
    event_t& operator=(const event_t&) = delete;

  private:
    scheduler_itf& _s;
    const callback_t _cb;
    bool _expiring = false;

    event_t* _next = nullptr;  //! link to next time event in a link-list
    uint32_t _ctr = 0;         //! time event down-counter
    uint32_t _interval = 0;    //! interval for periodic time event
};

class oneshot_event_t {
    using callback_t = ZOQ::utils::Callback<>;

  public:
    constexpr oneshot_event_t(scheduler_itf& s, const callback_t& cb) : _e{s, cb} {}
    void arm(uint32_t period) { _e.arm(period, 0); }
    void disarm() { _e.disarm(); }
    [[nodiscard]] bool armed() const { return _e.armed(); }

  private:
    event_t _e;
};

class periodic_event_t {
    using callback_t = ZOQ::utils::Callback<>;

  public:
    constexpr periodic_event_t(scheduler_itf& s, const callback_t& cb) : _e{s, cb} {}
    void arm(uint32_t period) { _e.arm(1, period); }
    void disarm() { _e.disarm(); }
    [[nodiscard]] bool armed() const { return _e.armed(); }

  private:
    event_t _e;
};

}  // namespace ZOQ