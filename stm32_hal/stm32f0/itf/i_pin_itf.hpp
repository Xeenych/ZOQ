#pragma once

namespace itf {

class i_pin_itf {
  public:
    virtual bool get() = 0;
    virtual ~i_pin_itf() = default;
};

}  // namespace itf
