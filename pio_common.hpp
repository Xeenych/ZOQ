#pragma once

namespace ZOQ::Stm32_HAL {

struct Pin {
    GPIO_TypeDef* port;
    uint8_t pin_num;
};

enum class pinState { Reset = GPIO_PIN_RESET, Set = GPIO_PIN_SET };
}  // namespace ZOQ::Stm32_HAL
