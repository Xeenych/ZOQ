#include "scheduler.hpp"

#include <cassert>

namespace ZOQ {

scheduler_t::scheduler_t(TIM_HandleTypeDef& htim) : _htim{htim}
{
    auto status = HAL_TIM_Base_Start_IT(&_htim);
    assert(HAL_OK == status);
}

void scheduler_t::add(event_t* e)
{
    e->_next = _head;
    _head = e;
}

void scheduler_t::tick()
{
    for (event_t* t = _head; t != nullptr; t = t->_next)
        t->tick();
}

}  // namespace ZOQ