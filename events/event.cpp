
#include "event.hpp"

#include "sys/irq_utils.hpp"

namespace ZOQ {

void event_t::tick()
{
    if (0 == _ctr)  // timer disarmed
        return;

    if (1 == _ctr) {
        _cb.execute();
        return;
    }
    --_ctr;
}

void event_t::arm(uint32_t ctr, uint32_t interval)
{
    critical_section_t s;
    _ctr = ctr;
    _interval = interval;
}

void event_t::disarm(void)
{
    critical_section_t s;
    _ctr = 0U;
    _interval = 0U;
}

}  // namespace ZOQ
