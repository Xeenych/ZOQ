#pragma once

#include <cstdint>
#include <span>

namespace ZOQ::itf {

class spi_itf {
  public:
    virtual void write(const std::span<const uint8_t>& data) = 0;
    virtual ~spi_itf() = default;
};

}  // namespace ZOQ::itf
