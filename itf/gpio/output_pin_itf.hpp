#pragma once

namespace ZOQ::itf {

class output_pin_itf {
  public:
    virtual void set() = 0;
    virtual void reset() = 0;
    virtual bool get() = 0;
    virtual ~output_pin_itf() = default;
};

}  // namespace ZOQ::itf
