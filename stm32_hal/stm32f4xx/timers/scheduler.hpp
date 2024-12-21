#pragma once
#include <cassert>

#include "ZOQ/scheduler/scheduler.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

class scheduler_t : public ZOQ::scheduler::scheduler_t {
  public:
    scheduler_t(TIM_HandleTypeDef& htim) : _htim{htim} {
        auto status = HAL_TIM_Base_Start_IT(&_htim);
        assert(HAL_OK == status);
    }

    scheduler_t(const scheduler_t&) = delete;
    scheduler_t& operator=(const scheduler_t&) = delete;

  private:
    TIM_HandleTypeDef& _htim;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
