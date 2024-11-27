#pragma once

#include "drv/itf/i_pin_itf.hpp"
#include "mcp23s17.hpp"

namespace peripherals {

// Входной пин на основе mcp23s17
// Реализует интерфейс i_pin_itf
class mcp23s17_i_pin_t : public itf::i_pin_itf {
  public:
    mcp23s17_i_pin_t(mcp23s17_t& drv, uint8_t pin, mcp23s17_t::port port) : _drv{drv}, _pin{pin}, _port{port}
    {
        _drv.set_dir(pin, port, mcp23s17_t::dir::input);
    }
    mcp23s17_i_pin_t(const mcp23s17_i_pin_t&) = delete;
    mcp23s17_i_pin_t& operator=(const mcp23s17_i_pin_t&) = delete;

    // Прочитать значение пина
    bool get() override { return _drv.get_output(_pin, _port); }

  private:
    mcp23s17_t& _drv;
    const uint8_t _pin;
    const mcp23s17_t::port _port;
};

}  // namespace peripherals