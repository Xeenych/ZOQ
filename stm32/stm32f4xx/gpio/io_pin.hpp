#pragma once

#include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/gpio.hpp"
#include "stm32f4xx.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;

class io_pin_t final : public io_pin_itf {
  public:
    constexpr io_pin_t(const pin_name_t& p) : _port(p.port), _pin(p.pin) {}
    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }

    io_pin_t(const io_pin_t&) = delete;
    io_pin_t& operator=(const io_pin_t&) = delete;

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
