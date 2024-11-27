#pragma once
#include <cstdint>
#include <span>

namespace ZOQ::itf {
class uart_itf {
  public:
    virtual void write(const std::span<const uint8_t>& data) = 0;
    virtual void read(const std::span<uint8_t>& data) = 0;
    virtual ~uart_itf() = default;

  private:
};

}  // namespace ZOQ::itf