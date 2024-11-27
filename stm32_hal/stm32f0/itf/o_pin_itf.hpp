#pragma once

namespace itf {

class o_pin_itf {
  public:
    virtual void set() = 0;
    virtual void reset() = 0;
    virtual ~o_pin_itf() = default;
};

}  // namespace itf
