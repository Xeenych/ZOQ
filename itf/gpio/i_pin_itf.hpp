#pragma once

namespace ZOQ::itf {

class i_pin_itf {
  public:
    [[nodiscard]] virtual bool get() const = 0;
    virtual ~i_pin_itf() = default;
};

}  // namespace ZOQ::itf
