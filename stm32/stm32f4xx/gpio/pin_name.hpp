#pragma once

#include "stm32f4xx.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
