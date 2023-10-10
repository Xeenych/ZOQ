#pragma once

#include <cstdint>

#include "ZOQ/callback.hpp"

namespace ZOQ::stm32::stm32f4xx::gpio::exti {

void register_handler(uint32_t pin, const callback_t& cb);

}  // namespace ZOQ::stm32::stm32f4xx::gpio::exti
