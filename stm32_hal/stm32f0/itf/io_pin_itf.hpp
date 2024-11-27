#pragma once

namespace itf {

class io_pin_itf {
  public:
    virtual bool get() = 0;
    virtual void set() = 0;
    virtual void reset() = 0;
    virtual void set_output() = 0;
    virtual void set_input() = 0;
    virtual ~io_pin_itf() = default;
};

}  // namespace itf
