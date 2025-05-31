#pragma once

#include <cstdint>

#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using ZOQ::itf::o_pin_itf;

class o_pin_t : public o_pin_itf {
  public:
    o_pin_t(GPIO_TypeDef* port, uint32_t pin, bool state) : _port{port}, _pin{pin} {
        state ? set() : reset();

        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
    }
    constexpr void set() override { _port->BSRR = _pin; }
    constexpr void reset() override { _port->BSRR = (_pin << 16U); }

    ~o_pin_t() override { HAL_GPIO_DeInit(_port, _pin); }

    o_pin_t(const o_pin_t&) = delete;
    o_pin_t& operator=(const o_pin_t&) = delete;

  private:
    GPIO_TypeDef* const _port;
    const uint32_t _pin;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
