#pragma once

#include "ZOQ/callback.hpp"
// #include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "ZOQ/itf/gpio/it_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/pin_name.hpp"
#include "callback.hpp"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_system.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;
using ZOQ::callback_t;

class io_pin_it_t final : public it_pin_itf {
  public:
    // mode:
    //    - LL_EXTI_TRIGGER_NONE
    //    - LL_EXTI_TRIGGER_RISING
    //    - LL_EXTI_TRIGGER_FALLING
    //    - LL_EXTI_TRIGGER_RISING_FALLING
    // pull:
    //      - LL_GPIO_PULL_NO
    //      - LL_GPIO_PULL_UP
    //      - LL_GPIO_PULL_DOWN
    // speed:
    //      - LL_GPIO_SPEED_FREQ_LOW
    //      - LL_GPIO_SPEED_FREQ_MEDIUM
    //      - LL_GPIO_SPEED_FREQ_HIGH
    //      - LL_GPIO_SPEED_FREQ_VERY_HIGH
    // line:
    //      - LL_EXTI_LINE_xx
    // irqn:
    //      - EXTI9_5_IRQn
    io_pin_it_t(const pin_name_t& p, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t line, IRQn_Type irqn,
                uint32_t priority)
        : _p{p} {
        LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);

        LL_GPIO_SetPinPull(p.port, p.pin, pull);
        LL_GPIO_SetPinSpeed(p.port, p.pin, speed);
        LL_GPIO_SetPinMode(p.port, p.pin, LL_GPIO_MODE_INPUT);

        LL_EXTI_InitTypeDef EXTI_InitStruct;
        EXTI_InitStruct.Line_0_31 = line;
        EXTI_InitStruct.LineCommand = ENABLE;
        EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
        EXTI_InitStruct.Trigger = mode;
        LL_EXTI_Init(&EXTI_InitStruct);

        NVIC_SetPriority(irqn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, 0));
        NVIC_EnableIRQ(irqn);
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
        _callback.execute();
    }

  private:
    const pin_name_t _p;
    callback_t _callback{};

    [[nodiscard]] bool is_interrupt() const { return (RESET != LL_EXTI_IsActiveFlag_0_31(_p.pin)); }
    void clear_interrupt() const { LL_EXTI_ClearFlag_0_31(_p.pin); }
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
