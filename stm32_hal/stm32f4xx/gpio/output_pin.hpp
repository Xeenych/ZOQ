#pragma once

#include "ZOQ/itf/gpio/output_pin_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using namespace ZOQ::itf;

class output_pin_t : public output_pin_itf {
  public:
    output_pin_t() = default;
    inline constexpr output_pin_t(GPIO_TypeDef* port, const uint32_t& pin) : _port(port), _pin(pin){};
    inline void set() override { _port->BSRR = _pin; }
    inline void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    inline bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
