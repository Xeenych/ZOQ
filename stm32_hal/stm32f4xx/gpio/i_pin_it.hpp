#pragma once

#include "ZOQ/itf/gpio/it_pin_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using ZOQ::itf::it_pin_itf;

class i_pin_it_t final : public it_pin_itf {
  public:
    // mode:
    //    - GPIO_MODE_IT_RISING
    //    - GPIO_MODE_IT_FALLING
    //    - GPIO_MODE_EVT_RISING_FALLING
    // pull:
    //    - GPIO_NOPULL
    //    - GPIO_PULLUP
    //    - GPIO_PULLDOWN
    i_pin_it_t(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t pull) : _port{port}, _pin{pin} {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = pin;
        GPIO_InitStruct.Mode = mode;
        GPIO_InitStruct.Pull = pull;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(port, &GPIO_InitStruct);
    }
    i_pin_it_t(const i_pin_it_t&) = delete;
    i_pin_it_t& operator=(const i_pin_it_t&) = delete;

    void set_callback(const callback_t& cb) override { _callback = cb; }

    [[nodiscard]] bool get() const { return ((_port->IDR & _pin) != GPIO_PIN_RESET); }

    ~i_pin_it_t() override { HAL_GPIO_DeInit(_port, _pin); }

    void EXTI_IRQHandler() { _callback.Execute(); }

  private:
    GPIO_TypeDef* _port;
    uint32_t _pin;
    callback_t _callback{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
