#pragma once

#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "gpio.hpp"
#include "stm32f4xx.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;

class o_pin_t final : public o_pin_itf {
  public:
    o_pin_t(const pin_name_t& p, output_t output_type, pull_t pull, speed_t speed) : _port(p.port), _pin(p.pin)
    {
        {
            uint32_t temp = _port->MODER;
            temp &= ~(0x03 << (_pin << _pin));
            temp |= (uint32_t)pin_mode_t::output << (_pin << _pin);  // output mode
            _port->MODER = temp;
        }

        {
            uint32_t temp = _port->PUPDR;
            temp &= ~(0x03 << (_pin << _pin));
            temp |= (uint32_t)pull << (_pin << _pin);  // pullup mode
            _port->PUPDR = temp;
        }

        {
            uint32_t temp = _port->OSPEEDR;
            temp &= ~(0x03 << (_pin << _pin));
            temp |= (uint32_t)speed << (_pin << _pin);  // speed
            _port->OSPEEDR = temp;
        }

        {
            uint32_t temp = _port->OTYPER;
            if (output_type == output_t::push_pull)
                temp &= ~(_pin);
            else
                temp |= (_pin);
            _port->OTYPER = temp;
        }
    }

    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
