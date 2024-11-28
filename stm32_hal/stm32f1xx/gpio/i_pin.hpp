#pragma once

#if __has_include("stm32f1xx_hal.h")

#include "itf/i_pin_itf.hpp"
#include "pin_name.hpp"
#include "stm32f1xx_hal.h"

namespace stm32f1xx::gpio {

class i_pin_t final : public itf::i_pin_itf {
  public:
    constexpr i_pin_t(const pin_name_t& p) : _port{p.port}, _pin{p.pin} {}

    i_pin_t(const i_pin_t&) = delete;
    i_pin_t& operator=(const i_pin_t&) = delete;

    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }
    void deinit()
    {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = _pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(_port, &GPIO_InitStruct);
    }

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
};

}  // namespace stm32f1xx::gpio

#endif