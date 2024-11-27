// Push-pull pin
#pragma once

#include <cstdint>

#include "drv/itf/io_pin_itf.hpp"
#include "pin_name.hpp"
#include "stm32f0xx_hal.h"

namespace stm32f0xx::gpio {

class io_pin_od_t : public itf::io_pin_itf {
  public:
    // Open-drain output pin
    io_pin_od_t(const pin_name_t& p) : _port(p.port), _pin(p.pin)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = p.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(p.port, &GPIO_InitStruct);
    }
    // Hi-Z mode
    void set() override { _port->BSRR = _pin; }
    // Output 0
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); };

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

    io_pin_od_t(const io_pin_od_t&) = delete;
    io_pin_od_t& operator=(const io_pin_od_t&) = delete;

  protected:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace stm32f0xx::gpio
