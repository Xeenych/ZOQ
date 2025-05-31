#pragma once
#include "o_pin.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

class nss_pin_t final : public o_pin_t {
  public:
    constexpr nss_pin_t(const pin_name_t& p) : o_pin_t(p) {}
    void deinit() {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = _pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(_port, &GPIO_InitStruct);
    }

    void init() {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = _pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(_port, &GPIO_InitStruct);
    }
};
}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
