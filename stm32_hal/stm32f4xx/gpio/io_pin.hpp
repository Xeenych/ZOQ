#pragma once

#include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using io_pin_itf = ZOQ::itf::io_pin_itf;

class io_pin_t final : public io_pin_itf {
  public:
    // output_type:
    //    - GPIO_MODE_OUTPUT_PP
    //    - GPIO_MODE_OUTPUT_OD
    // pull:
    //    - GPIO_NOPULL
    //    - GPIO_PULLUP
    //    - GPIO_PULLDOWN
    // speed:
    //    - GPIO_SPEED_FREQ_LOW
    //    - GPIO_SPEED_FREQ_MEDIUM
    //    - GPIO_SPEED_FREQ_HIGH
    //    - GPIO_SPEED_FREQ_VERY_HIGH
    io_pin_t(GPIO_TypeDef* port, uint32_t pin, uint32_t output_type, uint32_t pull, uint32_t speed, bool state)
        : _port{port}, _pin{pin} {
        state ? set() : reset();
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = pin;
        GPIO_InitStruct.Mode = output_type;
        GPIO_InitStruct.Pull = pull;
        GPIO_InitStruct.Speed = speed;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
    }
    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (_pin << 16U); }
    [[nodiscard]] bool get() const override { return ((_port->IDR & _pin) != static_cast<uint32_t>(GPIO_PIN_RESET)); }

    io_pin_t(const io_pin_t&) = delete;
    io_pin_t& operator=(const io_pin_t&) = delete;

    ~io_pin_t() override { HAL_GPIO_DeInit(_port, _pin); }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
