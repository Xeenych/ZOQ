#include "event.hpp"

#include "sys/irq_utils.hpp"
#include "sys/logging.hpp"

namespace ZOQ {

event_t::event_t(scheduler_itf& s, const callback_t& cb) : _s{s}, _cb{cb} { s.add(this); }
event_t::~event_t() {
    // NOT implemented
    //_s.remove(this);
}

void event_t::tick() {
    if (0 == _ctr)  // timer disarmed
        return;

    if (1 == _ctr) {
        _expiring = true;
        return;
    }

    --_ctr;
}

void event_t::arm(uint32_t ctr, uint32_t interval) {
    critical_section_t s;
    _ctr = ctr;
    _interval = interval;
    _expiring = false;
}

void event_t::disarm(void) {
    critical_section_t s;
    _ctr = 0U;
    _interval = 0U;
    _expiring = false;
}

}  // namespace ZOQ
