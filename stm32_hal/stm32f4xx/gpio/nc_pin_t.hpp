#pragma once

#include "ZOQ/itf/gpio/output_pin_itf.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::gpio {

using namespace ZOQ::itf;

class nc_pin_t : public output_pin_itf {
  public:
    inline void set() override {}
    inline void reset() override {}
    inline bool get() override { return false; }
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::gpio
