#pragma once

#include "ZOQ/callback.hpp"
#include "scheduler.hpp"

namespace ZOQ {

class scheduler_t;

class event_t {
  public:
    event_t(scheduler_t& s, const callback_t& cb);
    // Запускает отложенное событие
    // ctr - число тиков через которое выполнится отложенное событие
    // interval - интервал для перезагрузки таймера
    // arm(100, 0) - однокртаное выполнение события через 100 тиков
    // arm(100, 500) - выполнить событие черз 100 тиков, а потом выполнять каждые 500 тиков
    void arm(uint32_t ctr, uint32_t interval);
    void disarm();
    bool armed() const { return (0 != m_ctr); }

    // call this on SysTick interrupt
    void tick();

  private:
    const callback_t _cb;

    event_t* _next = nullptr;  //! link to next time event in a link-list
    uint32_t m_ctr = 0;        //! time event down-counter
    uint32_t m_interval = 0;   //! interval for periodic time event

    friend class scheduler_t;
};

}  // namespace ZOQ