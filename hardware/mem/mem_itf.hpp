#pragma once
#include <cstdint>
#include <cstdlib>
#include <span>

namespace peripherals {

class mem_itf {
  public:
    virtual bool write(size_t offset, std::span<const uint8_t> data) = 0;
    virtual bool read(size_t offset, std::span<uint8_t> data) = 0;
    virtual bool write_byte(size_t offset, uint8_t b) = 0;
    virtual bool read_byte(size_t offset, uint8_t& b) = 0;
    virtual size_t size() = 0;
    virtual ~mem_itf() = default;
};

}  // namespace peripherals
