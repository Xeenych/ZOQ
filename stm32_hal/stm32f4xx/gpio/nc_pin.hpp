#pragma once

#include "itf/i_pin_it_itf.hpp"
#include "itf/o_pin_itf.hpp"
#include "pin_name.hpp"
#include "stm32f4xx_hal.h"

namespace stm32f4xx::gpio {

class nc_pin_t final : public itf::o_pin_itf, public itf::i_pin_it_itf {
  public:
    constexpr nc_pin_t() {}
    constexpr void set() override {}
    constexpr void reset() override {}

    bool get() override { return false; }
    void set_callback(callback_t cb, void* ctx) override {}

  private:
};

}  // namespace stm32f4xx::gpio
