#pragma once
#include <span>

namespace itf {
class uart_itf {
  public:
    virtual void write(const std::span<const uint8_t>& data) = 0;
    virtual void read(const std::span<uint8_t>& data) = 0;
    virtual ~uart_itf() = default;

  private:
};

}  // namespace itf