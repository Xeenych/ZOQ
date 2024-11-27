#pragma once
#include "stm32f0xx_hal.h"

namespace stm32f0xx::gpio {

enum class pin_mode_t { input = 0x00, output = 0x01, alternate = 0x02, analog = 0x03 };

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

}  // namespace stm32f0xx::gpio
