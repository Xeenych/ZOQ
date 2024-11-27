
#define LOG_ENABLE 0
#include "scheduler.hpp"

#include "sys/dassert.h"
#include "sys/irq_utils.hpp"
#include "tim.h"

// Runs at 1ms
// Used to trigger transceiver operation and scheduler
// Не используем SysTick, чтобы в прерывании работала фукнция HAL_Delay и переадча по USB для логирования
// Необходимо поставить приоритет тоу прерыванию в 1, чтобы оно было менее приоритетным чем USB и SysTick
extern "C" void TIM17_IRQHandler(void)
{
    // LOG("HAL_TIM_PeriodElapsedCallback()");
    TIM_HandleTypeDef* htim = &htim17;
    // assert(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) == SET);
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);

    // trigger scheduler
    scheduler::event_t::tick();
}

namespace scheduler {

void init()
{
    // LOG("START TIM11");
    auto status = HAL_TIM_Base_Start_IT(&htim17);
    DASSERT(HAL_OK == status);
}

void event_t::arm(uint32_t ctr, uint32_t interval)
{
    // LOG("arm ctr:%d, interval:%d", ctr, interval);
    critical_section_t s;
    m_ctr = ctr;
    m_interval = interval;
}

void event_t::disarm(void)
{
    critical_section_t s;
    m_ctr = 0U;
    m_interval = 0U;
}

void event_t::tick(void)
{
    for (event_t* t = timeEvt_head; t != nullptr; t = t->m_next) {
        uint32_t val = save_interrupt();
        if (t->m_ctr == 0U) {  // disarmed? (most frequent case)
            restore_interrupt(val);
        } else if (t->m_ctr == 1U) {   // expiring?
            t->m_ctr = t->m_interval;  // reload timer
            restore_interrupt(val);
            t->execute();
        } else {  // timing out
            --t->m_ctr;
            restore_interrupt(val);
        }
    }
}

}  // namespace scheduler
