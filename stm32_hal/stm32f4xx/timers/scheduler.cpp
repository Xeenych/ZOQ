#include "scheduler.hpp"

#include <cassert>

namespace ZOQ::stm32_hal::stm32f4xx::timers {

scheduler_t::scheduler_t(TIM_HandleTypeDef& htim) : _htim{htim} {
    auto status = HAL_TIM_Base_Start_IT(&_htim);
    assert(HAL_OK == status);
}

void scheduler_t::add(event_t* e) {
    e->set_next(_head);
    _head = e;
}

void scheduler_t::tick() {
    for (event_t* t = _head; t != nullptr; t = t->next())
        t->tick();
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers