#pragma once
#include <cassert>

#include "event.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ {

class event_t;
class scheduler_t {
  public:
    scheduler_t(TIM_HandleTypeDef& htim);
    void tick();

  private:
    TIM_HandleTypeDef& _htim;
    event_t* _head = nullptr;

    void add(event_t* e);

    friend class event_t;
};

}  // namespace ZOQ
