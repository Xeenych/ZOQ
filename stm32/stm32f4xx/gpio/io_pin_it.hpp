#pragma once

#include "ZOQ/itf/gpio/it_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/pin_name.hpp"
#include "stm32f4xx_ll_exti.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;

class io_pin_it_t final : public it_pin_itf {
  public:
    // mode:
    //    - GPIO_MODE_IT_RISING
    //    - GPIO_MODE_IT_FALLING
    //    - GPIO_MODE_EVT_RISING_FALLING
    // pull:
    //    - GPIO_NOPULL
    //    - GPIO_PULLUP
    //    - GPIO_PULLDOWN
    io_pin_it_t(const pin_name_t& p, uint32_t mode, uint32_t pull) : _p{p} {
        GPIO_InitTypeDef GPIO_InitStruct{};
        GPIO_InitStruct.Pin = p.pin;
        GPIO_InitStruct.Mode = mode;
        GPIO_InitStruct.Pull = pull;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(p.port, &GPIO_InitStruct);
    }
    io_pin_it_t(const io_pin_it_t&) = delete;
    io_pin_it_t& operator=(const io_pin_it_t&) = delete;

    void set_callback(const callback_t& cb) override { _callback = cb; }

    constexpr void set() const { _p.port->BSRR = _p.pin; }
    constexpr void reset() const { _p.port->BSRR = (_p.pin << 16U); }
    [[nodiscard]] bool get() const { return ((_p.port->IDR & _p.pin) != GPIO_PIN_RESET); }

    ~io_pin_it_t() override { HAL_GPIO_DeInit(_p.port, _p.pin); }

    void EXTI_IRQHandler() {
        if (!is_interrupt())
            return;
        clear_interrupt();
        _callback.Execute();
    }

  private:
    const pin_name_t _p;
    callback_t _callback{};

    //[[nodiscard]] bool is_interrupt() const { return (RESET != __HAL_GPIO_EXTI_GET_IT(_p.pin)); }
    // void clear_interrupt() const { __HAL_GPIO_EXTI_CLEAR_IT(_p.pin); }

    [[nodiscard]] bool is_interrupt() const { return (RESET != LL_EXTI_IsActiveFlag_0_31(_p.pin)); }
    void clear_interrupt() const { LL_EXTI_ClearFlag_0_31(_p.pin); }
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
