#pragma once

#include <cstdint>

#include "itf/io_pin_itf.hpp"
#include "pin_name.hpp"
#include "stm32f0xx_hal.h"

namespace stm32f0xx::gpio {

class io_pin_t : public itf::io_pin_itf {
  public:
    constexpr io_pin_t(const pin_name_t& p) : _port(p.port), _pin(p.pin) {}
    constexpr void set() override { _port->BSRR = _pin; }
    constexpr void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }
    void set_input() override
    {
        uint32_t temp = _port->MODER;
        temp &= ~(0x03 << (_pin << _pin));
        temp |= (uint32_t)pin_mode_t::input << (_pin << _pin);  // input mode
        _port->MODER = temp;
    }
    void set_output() override
    {
        uint32_t temp = _port->MODER;
        temp &= ~(0x03 << (_pin << _pin));
        temp |= (uint32_t)pin_mode_t::output << (_pin << _pin);  // output mode
        _port->MODER = temp;
    }

    io_pin_t(const io_pin_t&) = delete;
    io_pin_t& operator=(const io_pin_t&) = delete;

  protected:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace stm32f0xx::gpio
