#pragma once

#include <cstdint>

#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/pin_name.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {
using namespace ZOQ::stm32::stm32f4xx::gpio;

class o_pin_t : public itf::o_pin_itf {
  public:
    o_pin_t(const pin_name_t& p, bool state) : _port(p.port), _pin(p.pin) {
        if (state)
            set();
        else
            reset();

        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = p.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(p.port, &GPIO_InitStruct);
    }
    constexpr void set() override { _port->BSRR = _pin; }
    constexpr void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }

    ~o_pin_t() { HAL_GPIO_DeInit(_port, _pin); }

    o_pin_t(const o_pin_t&) = delete;
    o_pin_t& operator=(const o_pin_t&) = delete;

  protected:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
