
#include "scheduler.hpp"

#include "sys/dassert.h"
#include "sys/irq_utils.hpp"

namespace ZOQ {

void scheduler_t::tick()
{
    for (event_t* t = _head; t != nullptr; t = t->_next)
        t->tick();
}

void event_t::tick()
{
    uint32_t val = save_interrupt();
    if (m_ctr == 0U) {  // disarmed? (most frequent case)
        restore_interrupt(val);
    } else if (m_ctr == 1U) {  // expiring?
        m_ctr = m_interval;    // reload timer
        restore_interrupt(val);
        execute();
    } else {  // timing out
        --m_ctr;
        restore_interrupt(val);
    }
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

}  // namespace ZOQ
