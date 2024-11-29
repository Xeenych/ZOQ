#pragma once

#include "ZOQ/itf/gpio/o_pin_itf.hpp"

namespace ZOQ::hardware::led {

template <bool inverted = false>
class led_t : public itf::o_pin_itf {
  public:
    constexpr led_t(o_pin_itf& pin) : _pin{pin} {}
    void set() { inverted ? _pin.reset() : _pin.set(); }
    void reset() { inverted ? _pin.set() : _pin.reset(); }

    led_t(const led_t&) = delete;
    led_t& operator=(const led_t&) = delete;

  private:
    o_pin_itf& _pin;
};

}  // namespace ZOQ::hardware::led
