#pragma once

#include "drv/itf/i2c_itf.hpp"
#include "mem_itf.hpp"

namespace peripherals {

template <uint8_t addr, size_t sz>
class i2c_mem_t : public mem_itf {
  public:
    constexpr i2c_mem_t(itf::i2c_itf& i2c) : _i2c{i2c} {}
    bool write(size_t offset, std::span<const uint8_t> data) override
    {
        return (data.size_bytes() + offset <= sz) ? _i2c.write(addr, offset, data) : false;
    }

    bool read(size_t offset, std::span<uint8_t> data) override
    {
        return (data.size_bytes() + offset <= sz) ? _i2c.read(addr, offset, data) : false;
    }

    bool write_byte(size_t offset, uint8_t b) override
    {
        return (offset + 1 <= sz) ? _i2c.write_byte(addr, offset, b) : false;
    }
    bool read_byte(size_t offset, uint8_t& b) override
    {
        return (offset + 1 <= sz) ? _i2c.read_byte(addr, offset, b) : false;
    }

    size_t size() override { return sz; }

    i2c_mem_t(const i2c_mem_t&) = delete;
    i2c_mem_t& operator=(const i2c_mem_t&) = delete;

  private:
    itf::i2c_itf& _i2c;
};

}  // namespace peripherals
