#pragma once
#if __has_include("stm32f4xx_hal.h")
extern "C" {
#include "stm32f4xx_hal.h"
}

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio

#endif
