#pragma once

#include "ZOQ/itf/gpio/o_pin_itf.hpp"
#include "stm32f4xx_ll_gpio.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using ZOQ::itf::o_pin_itf;

class o_pin_t final : public o_pin_itf {
  public:
    // output_type:
    // * LL_GPIO_OUTPUT_PUSHPULL
    // * LL_GPIO_OUTPUT_OPENDRAIN
    // pull:
    // * LL_GPIO_PULL_NO
    // * LL_GPIO_PULL_UP
    // * LL_GPIO_PULL_DOWN
    // speed:
    // * LL_GPIO_SPEED_FREQ_LOW
    // * LL_GPIO_SPEED_FREQ_MEDIUM
    // * LL_GPIO_SPEED_FREQ_HIGH
    // * LL_GPIO_SPEED_FREQ_VERY_HIGH
    o_pin_t(GPIO_TypeDef* port, uint32_t pin, uint32_t output_type, uint32_t pull, uint32_t speed)
        : _port{port}, _pin{pin} {
        LL_GPIO_SetPinOutputType(port, pin, output_type);
        LL_GPIO_SetPinPull(port, pin, pull);
        LL_GPIO_SetPinSpeed(port, pin, speed);
        LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    }

    o_pin_t(const o_pin_t&) = delete;
    o_pin_t& operator=(const o_pin_t&) = delete;

    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (_pin << 16U); }

  private:
    GPIO_TypeDef* const _port;
    const uint32_t _pin;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
