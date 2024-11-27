#pragma once

#if __has_include("stm32f1xx_hal.h")
extern "C" {
#include "stm32f1xx_hal.h"
}

namespace stm32f1xx::gpio {

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

}  // namespace stm32f1xx::gpio

#endif
