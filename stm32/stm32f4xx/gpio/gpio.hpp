#pragma once

#include "stm32f4xx.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

struct pin_name_t {
    GPIO_TypeDef* port;
    uint32_t pin;
};

enum class pin_mode_t { input = 0x00, output = 0x01, alternate = 0x02, analog = 0x03 };
enum class output_t { push_pull = 0x00, open_drain = 0x01 };
enum class pull_t { no_pull = 0x00, pull_up = 0x01, pull_down = 0x02 };
enum class speed_t { low = 0x00, medium = 0x01, high = 0x02, very_high = 0x03 };

}  // namespace ZOQ::stm32::stm32f4xx::gpio
