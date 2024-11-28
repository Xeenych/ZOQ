#pragma once

#if __has_include("stm32f4xx.h")
extern "C" {
#include "stm32f4xx.h"
}
#include <cstdint>

namespace ZOQ::stm32::stm32f4xx::gpio {

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio

#endif
