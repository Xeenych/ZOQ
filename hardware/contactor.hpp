#pragma once

#include "itf/gpio/io_pin_itf.hpp"

namespace ZOQ::Hardware {

using namespace ZOQ::itf;

class contactor {
  public:
    inline contactor(io_pin_itf& _drv, io_pin_itf& _status);
    inline void set() const;
    inline void reset() const;
    inline bool isSet() const;

  private:
    io_pin_itf& driver_pin;
    io_pin_itf& status_pin;
};

void contactor::set() const
{
    driver_pin.set();
}

contactor::contactor(io_pin_itf& _drv, io_pin_itf& _status) : driver_pin(_drv), status_pin(_status)
{
    contactor::reset();
}

void contactor::reset() const
{
    driver_pin.reset();
}

bool contactor::isSet() const
{
    return status_pin.get();
}
}  // namespace ZOQ::Hardware