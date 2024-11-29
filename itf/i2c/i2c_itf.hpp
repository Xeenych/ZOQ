#pragma once

#include <cstdint>
#include <span>

namespace itf {

class i2c_itf {
  public:
    // false - no ACK
    virtual bool write(uint8_t slave_addr, uint8_t reg, const std::span<const uint8_t>& data) = 0;
    // false - no ACK
    virtual bool read(uint8_t slave_addr, uint8_t reg, const std::span<uint8_t>& data) = 0;
    // false - no ACK
    virtual bool read_byte(uint8_t slave_addr, uint8_t addr, uint8_t& b) = 0;
    // false - no ACK
    virtual bool write_byte(uint8_t slave_addr, uint8_t addr, const uint8_t& b) = 0;

    virtual ~i2c_itf() = default;
};

}  // namespace itf
