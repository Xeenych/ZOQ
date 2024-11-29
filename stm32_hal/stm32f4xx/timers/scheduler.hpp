#pragma once
#include <cassert>
#include <cstdint>

#include "event.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ {

class event_t;
class scheduler_t {
  public:
    scheduler_t(TIM_HandleTypeDef& htim);
    void tick();
    void add(event_t* e);

  private:
    TIM_HandleTypeDef& _htim;
    event_t* _head = nullptr;
};

}  // namespace ZOQ
