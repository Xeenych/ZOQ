// Push-pull pin
#pragma once

#include <cstdint>

#include "drv/itf/o_pin_itf.hpp"
#include "pin_name.hpp"
#include "stm32f0xx_hal.h"

namespace stm32f0xx::gpio {

class o_pin_pp_t : public itf::o_pin_itf {
  public:
    // Push-pull output pin
    o_pin_pp_t(const pin_name_t& p) : _port(p.port), _pin(p.pin)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = p.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(p.port, &GPIO_InitStruct);
    }
    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }

    o_pin_pp_t(const o_pin_pp_t&) = delete;
    o_pin_pp_t& operator=(const o_pin_pp_t&) = delete;

  protected:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace stm32f0xx::gpio
