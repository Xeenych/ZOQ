/*
    Программный драйвер i2c.
    Блокирующий
*/
#pragma once

#include "drv/itf/i2c_itf.hpp"
#include "drv/itf/io_pin_itf.hpp"
#include "drv/itf/o_pin_itf.hpp"

namespace peripherals {
using itf::io_pin_itf;
using itf::o_pin_itf;

// Софт-драйвер i2c
class software_i2c_t : public itf::i2c_itf {
  public:
    software_i2c_t(size_t baud, io_pin_itf& sda, o_pin_itf& scl);
    // Блокрирующая функция
    // Возвращает false, если не получен ACK
    bool write(uint8_t slave_addr, uint8_t reg, const std::span<const uint8_t>& data) override;
    // Блокрирующая функция
    // Возвращает false, если не получен ACK
    bool read(uint8_t slave_addr, uint8_t reg, const std::span<uint8_t>& data) override;

    bool read_byte(uint8_t slave_addr, uint8_t addr, uint8_t& b) override;
    bool write_byte(uint8_t slave_addr, uint8_t addr, const uint8_t& b) override;

    software_i2c_t(const software_i2c_t&) = delete;
    software_i2c_t& operator=(const software_i2c_t&) = delete;

  private:
    const size_t _delay;
    io_pin_itf& _sda;
    o_pin_itf& _scl;

    void sdaLo();
    void sdaHi();
    void sclLo();
    void sclHi();
    void start();
    void stop();

    void write1byte(uint8_t byte);
    uint8_t read1byte();
    void read_op(uint8_t byte);
    void write_op(uint8_t byte);
    bool checkAckBit();
    void writeAck();
    bool read_reg(uint8_t slave_addr, uint8_t reg, uint8_t& data);
    bool write_reg(uint8_t slave_addr, uint8_t reg, uint8_t data);
    void delay1();
    void delay2();
};

}  // namespace peripherals
