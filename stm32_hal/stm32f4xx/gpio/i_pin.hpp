#pragma once

#include "ZOQ/itf/gpio/i_pin_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

class i_pin_t final : public itf::i_pin_itf {
  public:
    constexpr i_pin_t(GPIO_TypeDef* port, uint32_t pin) : _port{port}, _pin{pin} {}

    i_pin_t(const i_pin_t&) = delete;
    i_pin_t& operator=(const i_pin_t&) = delete;

    bool get() override { return ((_port->IDR & _pin) != static_cast<uint32_t>(GPIO_PIN_RESET)); }
    void deinit() {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = _pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(_port, &GPIO_InitStruct);
    }

  private:
    GPIO_TypeDef* const _port;
    const uint32_t _pin;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
