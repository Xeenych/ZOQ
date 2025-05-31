#pragma once

#include "itf/gpio/it_pin_itf.hpp"
#include "itf/gpio/o_pin_itf.hpp"
#include "itf/gpio/i_pin_itf.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using ZOQ::itf::i_pin_itf;
using ZOQ::itf::it_pin_itf;
using ZOQ::itf::o_pin_itf;

class nc_pin_t final : public o_pin_itf, public i_pin_itf, public it_pin_itf {
  public:
    constexpr nc_pin_t() = default;
    constexpr void set() override {}
    constexpr void reset() override {}

    [[nodiscard]] bool get() const override { return false; }
    void set_callback(const callback_t& cb) override {}
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
