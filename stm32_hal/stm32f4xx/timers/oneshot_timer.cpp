#define LOG_ENABLE 0
#include "oneshot_timer.hpp"

#include <cassert>

#include "HAL_TIM_interrupt_handler.hpp"
#include "logging.h"
#include "tim.h"

using namespace ZOQ;

static ZOQ::stm32_hal::stm32f4xx::timers::oneshot_timer_t::event_t event_table[10];

bool flag = 0;

extern "C" void TIM1_TRG_COM_TIM11_IRQHandler()
{
    TIM_HandleTypeDef* htim = &htim11;
    LOG_DBG("HAL_TIM_PeriodElapsedCallback()");
    assert(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) == SET);
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);

    for (auto& ev : event_table)
        if (ev._cb) {
            ev._ticks--;
            if (0 == ev._ticks) {
                auto cb = ev._cb;
                ev._cb = nullptr;  // deregister event
                cb->execute();
            }
        }

    LOG_DBG("HAL_TIM_PeriodElapsedCallback() END");
    // Check that no events are missed
    assert(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) == RESET);
}

namespace ZOQ::stm32_hal::stm32f4xx::timers {

oneshot_timer_t::oneshot_timer_t(TIM_HandleTypeDef& htim) : _htim(htim)
{
    LOG_DBG("oneshot_timer_t()");
    auto status = HAL_TIM_Base_Start_IT(&_htim);
    assert(HAL_OK == status);
}

oneshot_timer_t::~oneshot_timer_t()
{
    LOG_DBG("~oneshot_timer_t()");
    auto status = HAL_TIM_Base_Stop_IT(&_htim);
    assert(HAL_OK == status);
}

void oneshot_timer_t::register_event(callback_t* cb, size_t ticks)
{
    LOG_DBG("register_event()");
    __disable_irq();
    for (auto& el : event_table)
        if (!el._cb) {
            el = {cb, ticks};
            __enable_irq();
            return;
        }
    LOG_DBG("ASSERT");
    assert(false);
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers