#pragma once

#include <cstdint>

#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/pin_name.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {
using namespace ZOQ::stm32::stm32f4xx::gpio;

class o_pin_t : public itf::o_pin_itf {
  public:
    constexpr o_pin_t(const pin_name_t& p) : _port(p.port), _pin(p.pin) {}
    constexpr void set() override { _port->BSRR = _pin; }
    constexpr void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }

    o_pin_t(const o_pin_t&) = delete;
    o_pin_t& operator=(const o_pin_t&) = delete;

  protected:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
