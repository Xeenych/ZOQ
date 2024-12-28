#pragma once

#include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "pin_name.hpp"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;
using namespace ZOQ::stm32::stm32f4xx::gpio;

class io_pin_t final : public io_pin_itf {
  public:
    io_pin_t(const pin_name_t& p, bool state) : _port(p.port), _pin(p.pin) {
        if (state)
            set();
        else
            reset();

        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = p.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(p.port, &GPIO_InitStruct);
    }
    void set() override { _port->BSRR = _pin; }
    void reset() override { _port->BSRR = (uint32_t)(_pin << 16U); }
    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }

    io_pin_t(const io_pin_t&) = delete;
    io_pin_t& operator=(const io_pin_t&) = delete;

    ~io_pin_t() { HAL_GPIO_DeInit(_port, _pin); }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
