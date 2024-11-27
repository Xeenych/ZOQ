/*
    Заглушка драйвера i2c
*/
#pragma once

#include "drv/itf/i2c_itf.hpp"

namespace peripherals {

class dummy_i2c_t : public itf::i2c_itf {
  public:
    bool write(uint8_t slave_addr, uint8_t reg, const std::span<const uint8_t>& data) override { return true; }
    bool read(uint8_t slave_addr, uint8_t reg, const std::span<uint8_t>& data) override { return true; }
    bool read_byte(uint8_t slave_addr, uint8_t addr, uint8_t& b) override { return true; }
    bool write_byte(uint8_t slave_addr, uint8_t addr, const uint8_t& b) override { return true; }

  private:
};

}  // namespace peripherals
