#pragma once

#include "drv/itf/o_pin_itf.hpp"

namespace stm32f4xx::gpio {

class nc_pin_t final : public itf::o_pin_itf {
  public:
    constexpr nc_pin_t() {}
    void set() override {};
    void reset() override {};

  private:
};

}  // namespace stm32f4xx::gpio
