#pragma once

#include "itf/gpio/i_pin_it_itf.hpp"
#include "itf/gpio/o_pin_itf.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

class nc_pin_t final : public itf::o_pin_itf, public itf::i_pin_it_itf {
  public:
    constexpr nc_pin_t() = default;
    constexpr void set() override {}
    constexpr void reset() override {}

    bool get() override { return false; }
    void set_callback(callback_t cb, void* ctx) override {}

  private:
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
