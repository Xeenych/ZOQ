#pragma once

#include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "stm32f4xx.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;

class io_pin_t final : public io_pin_itf {
  public:
    constexpr io_pin_t(const pin_def_t& p) : _port(static_cast<GPIO_TypeDef*>(p.port)), _pin(p.pin){};
    // constexpr io_pin_t(GPIO_TypeDef* port, const uint32_t& pin) : _port(port), _pin(pin){};
    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
