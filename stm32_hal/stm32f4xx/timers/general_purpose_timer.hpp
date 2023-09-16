#pragma once
#include "ZOQ/itf/timers/delay_timer_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ::itf;

class general_purpose_timer_t : public delay_timer_itf {
  public:
    general_purpose_timer_t(TIM_HandleTypeDef& htim);
    void delay_ticks(size_t ticks) override;
    ~general_purpose_timer_t() override;

  private:
    TIM_HandleTypeDef& _htim;
};
}  // namespace ZOQ::stm32_hal::stm32f4xx::timers