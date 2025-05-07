#pragma once

#include <cstdint>
#include <span>

namespace ZOQ::itf {

class spi_itf {
  public:
    virtual void write(const std::span<const uint8_t>& data) = 0;
    virtual void read(const std::span<uint8_t>& data) = 0;
    virtual uint8_t readwrite(uint8_t byte) = 0;
    virtual void readwrite(const std::span<uint8_t>& data, const std::span<uint8_t>& dataout) = 0;
    virtual ~spi_itf() = default;
};

}  // namespace ZOQ::itf
