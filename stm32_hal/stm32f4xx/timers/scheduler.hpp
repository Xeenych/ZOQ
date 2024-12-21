#pragma once
#include <cassert>

#include "ZOQ/events/event.hpp"
#include "ZOQ/itf/timers/scheduler_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

class scheduler_t : public scheduler_itf {
  public:
    scheduler_t(TIM_HandleTypeDef& htim);
    void tick();

    scheduler_t(const scheduler_t&) = delete;
    scheduler_t& operator=(const scheduler_t&) = delete;

  private:
    TIM_HandleTypeDef& _htim;
    event_t* _head = nullptr;

    void add(event_t* e) override;

    friend class event_t;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
