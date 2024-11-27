#pragma once

#include "itf/i_pin_it_itf.hpp"
#include "pin_name.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_exti.h"

namespace stm32f1xx::gpio {

// Класс пина, работающего по прерыванию
class i_pin_it_t final : public itf::i_pin_it_itf {
  public:
    constexpr i_pin_it_t(const pin_name_t& p, IRQn_Type irqn, uint32_t priority)
        : _port{p.port}, _pin{p.pin}, _irqn{irqn} {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = _pin;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(_port, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(_irqn, priority, 0);
        enable();
    }

    bool get() override { return ((_port->IDR & _pin) != (uint32_t)GPIO_PIN_RESET); }

    void enable() {
        // LL_EXTI_EnableIT_0_31(_pin);
        HAL_NVIC_EnableIRQ(_irqn);
    }
    void disable() {
        // LL_EXTI_DisableIT_0_31(_pin);
        HAL_NVIC_DisableIRQ(_irqn);
    }

    constexpr void set_callback(callback_t cb, void* ctx) override {
        _cb = cb;
        _ctx = ctx;
    }

    // Эту функцию надо вызывать из вектора прерывания
    void on_interrupt() {
        if (!is_interrupt())
            return;
        clear_interrupt();
        if (_cb)
            _cb(_ctx);
    }

    i_pin_it_t(const i_pin_it_t&) = delete;
    i_pin_it_t& operator=(const i_pin_it_t&) = delete;

  private:
    GPIO_TypeDef* const _port = nullptr;
    const uint32_t _pin = 0;
    const IRQn_Type _irqn;

    callback_t _cb = nullptr;
    void* _ctx = nullptr;

    bool is_interrupt() { return (RESET != __HAL_GPIO_EXTI_GET_IT(_pin)); }
    void clear_interrupt() { __HAL_GPIO_EXTI_CLEAR_IT(_pin); }
};

}  // namespace stm32f1xx::gpio
