#pragma once
#include <cassert>
#include <cstdint>

#include "stm32f4xx_hal.h"

namespace ZOQ {

class event_t;
class scheduler_t {
  public:
    scheduler_t(TIM_HandleTypeDef& htim);
    void tick();
    constexpr void add(event_t* e);

  private:
    TIM_HandleTypeDef& _htim;
    event_t* _head = nullptr;
};

class event_t {
  public:
    using fn_t = void (*)(void* arg);
    event_t(scheduler_t& s, const fn_t& f, void* arg) : _fn{f}, _arg{arg} { s.add(this); }
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
    const fn_t _fn = nullptr;
    void* const _arg = nullptr;

    event_t* _next = nullptr;  //! link to next time event in a link-list
    uint32_t m_ctr = 0;        //! time event down-counter
    uint32_t m_interval = 0;   //! interval for periodic time event

    void execute()
    {
        if (_fn)
            _fn(_arg);
    }

    friend class scheduler_t;
};

inline scheduler_t::scheduler_t(TIM_HandleTypeDef& htim) : _htim{htim}
{
    auto status = HAL_TIM_Base_Start_IT(&_htim);
    assert(HAL_OK == status);
}

constexpr inline void scheduler_t::add(event_t* e)
{
    _head = e;
    e->_next = _head;
}

inline void scheduler_t::tick()
{
    for (event_t* t = _head; t != nullptr; t = t->_next)
        t->tick();
}

}  // namespace ZOQ
