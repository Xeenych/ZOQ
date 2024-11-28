#pragma once

#include "ZOQ/callback.hpp"
// #include "ZOQ/itf/gpio/io_pin_itf.hpp"
#include "ZOQ/itf/gpio/it_pin_itf.hpp"
#include "ZOQ/stm32/stm32f4xx/gpio/pin_name.hpp"
#include "stm32f4xx_ll_exti.h"

namespace ZOQ::stm32::stm32f4xx::gpio {

using namespace ZOQ::itf;

class io_pin_it_t final : public it_pin_itf {
  public:
    constexpr io_pin_it_t(const pin_name_t& p) : _p{p} {}

    void set_callback(const callback_t& cb) { _callback = cb; }

    constexpr void set()  { _p.port->BSRR = _p.pin; }
    constexpr void reset()  { _p.port->BSRR = (uint32_t)(_p.pin << 16U); }
    bool get()  { return ((_p.port->IDR & _p.pin) != (uint32_t)GPIO_PIN_RESET); }

    void EXTI_IRQHandler()
    {
        if (!LL_EXTI_IsActiveFlag_0_31(_p.pin))
            return;
        LL_EXTI_ClearFlag_0_31(_p.pin);
        _callback.execute();
    }

  private:
    const pin_name_t _p;
    callback_t _callback{};

    io_pin_it_t(const io_pin_it_t&) = delete;
    io_pin_it_t& operator=(const io_pin_it_t&) = delete;
};

}  // namespace ZOQ::stm32::stm32f4xx::gpio
