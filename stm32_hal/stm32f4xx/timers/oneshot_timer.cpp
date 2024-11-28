#define LOG_ENABLE 0
#include "oneshot_timer.hpp"

#include "ZOQ/stm32_hal/critical_section.hpp"
#include "logging.h"

// using namespace ZOQ;
using namespace ZOQ::stm32_hal::stm32f4xx::timers;

namespace ZOQ::stm32_hal::stm32f4xx::timers {

void oneshot_timer_t::on_interrupt()
{
    if (RESET == __HAL_TIM_GET_FLAG(&_htim, TIM_FLAG_UPDATE))
        return;
    __HAL_TIM_CLEAR_IT(&_htim, TIM_IT_UPDATE);

    for (auto& ev : event_table) {
        if (!ev.valid())
            continue;
        ev._ticks_left--;
        if (ev.expired()) {
            auto e = ev;
            ev.clear();
            e.execute();
        }
    }

    // Check that no events are missed
    // assert(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) == RESET);
}

oneshot_timer_t::~oneshot_timer_t()
{
    LOG_DBG("~oneshot_timer_t()");
    auto status = HAL_TIM_Base_Stop_IT(&_htim);
    assert(HAL_OK == status);

    for (auto& el : event_table)
        el.clear();
}

void oneshot_timer_t::schedule(const callback_t& cb, size_t ticks)
{
    LOG_DBG("schedule()");
    {
        critical_section_t sec;
        for (auto& el : event_table)
            if (!el.valid()) {
                el = {cb, ticks};
                return;
            }
    }
    assert(false);
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
