#pragma once

#include "ZOQ/callback.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::timers {

using namespace ZOQ;

class HAL_TIM_interrupt_handler_t {
  public:
    // takes a pointer to callback_t struct
    // the caller is responsible for lifetime of cb
    static void register_handler(const callback_t* cb);

    // takes a pointer to callback_t struct
    // the caller is responsible for lifetime of cb
    static void unregister_handler(const callback_t* cb);

    static void on_interrupt();

  private:
    static inline callback_t handlers[10]{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::timers
