#include "software_i2c.hpp"

extern "C" {
#include "cmsis_compiler.h"
}

#include "sys/logging.hpp"

namespace peripherals {

void software_i2c_t::delay1()
{
    for (size_t i = 0; i < _delay; i++)  // _delay=12 = 98kHzs @ -Os optimisation
        __NOP();
}

void software_i2c_t::delay2()
{
    delay1();
    delay1();
}

void software_i2c_t::sdaLo()
{
    _sda.reset();
    delay1();
}

void software_i2c_t::sdaHi()
{
    _sda.set();
    delay1();
}

void software_i2c_t::sclLo()
{
    _scl.reset();
    delay1();
}

void software_i2c_t::sclHi()
{
    _scl.set();
    delay1();
}

software_i2c_t::software_i2c_t(size_t delay, io_pin_itf& sda, o_pin_itf& scl) : _delay{delay}, _sda{sda}, _scl{scl}
{
    sclHi();
    sdaHi();
}

bool software_i2c_t::write(uint8_t slave_addr, uint8_t reg, const std::span<const uint8_t>& data)
{
    start();
    write_op(slave_addr);
    if (!checkAckBit()) {
        stop();
        return false;  // Immediately end transmission and return 0 if NACK detected
    }
    write1byte(reg);
    if (!checkAckBit()) {
        stop();
        return 0;  // Immediately end transmission and return 0 if NACK detected
    }
    // Loop through bytes in the buffer
    for (uint8_t i = 0; i < data.size(); i++) {
        write1byte(data[i]);  // LSB
        if (!checkAckBit()) {
            stop();
            return 0;  // Immediately end transmission and return 0 if NACK detected
        }
    }
    stop();
    return true;
}

bool software_i2c_t::read(uint8_t slave_addr, uint8_t reg, const std::span<uint8_t>& data)
{
    start();
    write_op(slave_addr);  // 0 == Write bit
    if (!checkAckBit()) {
        stop();
        return 0;  // Immediately end transmission and return 0 if NACK detected
    }
    write1byte(reg);
    if (!checkAckBit()) {
        stop();
        return 0;  // Immediately end transmission and return 0 if NACK detected
    }
    start();
    read_op(slave_addr);  // 1 == Read bit
    if (!checkAckBit()) {
        stop();
        return false;  // Immediately end transmission and return 0 if NACK detected
    }
    // Loop through bytes in the buffer
    for (uint8_t i = 0; i < data.size(); i++) {
        data[i] = read1byte();
        if (i < (data.size() - 1))
            writeAck();
        else                // Last byte needs a NACK
            checkAckBit();  // Controller needs to send NACK when done reading data
    }

    stop();
    return true;  // Return 1 if no NACKs
}

bool software_i2c_t::write_reg(uint8_t slave_addr, uint8_t reg, uint8_t data)
{
    start();
    write_op(slave_addr);
    if (!checkAckBit()) {
        stop();  // Immediately end transmission and return 0 if NACK detected
        return false;
    }
    write1byte(reg);
    if (!checkAckBit()) {
        stop();  // Immediately end transmission and return 0 if NACK detected
        return false;
    }

    write1byte(data);
    if (!checkAckBit()) {
        stop();  // Immediately end transmission and return 0 if NACK detected
        return false;
    }
    stop();
    return true;
}

bool software_i2c_t::read_reg(uint8_t slave_addr, uint8_t reg, uint8_t& data)
{
    start();
    write_op(slave_addr);
    if (!checkAckBit()) {
        stop();
        return false;  // Immediately end transmission and return 0 if NACK detected
    }
    write1byte(reg);
    if (!checkAckBit()) {
        stop();
        return false;  // Immediately end transmission and return 0 if NACK detected
    }
    start();
    read_op(slave_addr);
    if (!checkAckBit()) {
        stop();
        return false;  // Immediately end transmission and return 0 if NACK detected
    }
    data = read1byte();
    checkAckBit();  // Controller needs to send NACK when done reading data
    stop();
    return true;
}

bool software_i2c_t::read_byte(uint8_t slave_addr, uint8_t mem_addr, uint8_t& byte)
{
    // auto res = immediate_read(slave_addr, byte);
    //  auto res = selective_read(slave_addr, mem_addr, byte);
    auto res = read_reg(slave_addr, mem_addr, byte);
    return res;
}

bool software_i2c_t::write_byte(uint8_t slave_addr, uint8_t addr, const uint8_t& b)
{
    return write_reg(slave_addr, addr, b);
}

// An SDA transition while SCL is
// high will be interpreted as a START or STOP condition
void software_i2c_t::start()
{
    sclHi();
    sdaLo();
    sclLo();
}

void software_i2c_t::stop()
{
    sdaLo();
    sclHi();
    sdaHi();
}

void software_i2c_t::read_op(uint8_t byte)
{
    write1byte(byte | 0x01);
}

void software_i2c_t::write_op(uint8_t byte)
{
    write1byte(byte & 0xfe);
}

// During data transfer, the SDA line must remain stable
// while the SCL line is high
void software_i2c_t::write1byte(uint8_t data)
{
    if (data & 0x80)
        sdaHi();  // bit 7
    else
        sdaLo();
    sclHi();
    sclLo();

    if (data & 0x40)
        sdaHi();  // bit 6
    else
        sdaLo();
    sclHi();
    sclLo();

    if (data & 0x20)
        sdaHi();  // bit 5
    else
        sdaLo();
    sclHi();
    sclLo();

    if (data & 0x10)
        sdaHi();  // bit 4
    else
        sdaLo();
    sclHi();
    sclLo();

    if (data & 0x08)
        sdaHi();  // bit 3
    else
        sdaLo();
    sclHi();
    sclLo();
    if (data & 0x04)
        sdaHi();  // bit 2
    else
        sdaLo();
    sclHi();
    sclLo();
    if (data & 0x02)
        sdaHi();  // bit 1
    else
        sdaLo();
    sclHi();
    sclLo();
    if (data & 0x01)
        sdaHi();  // bit 0
    else
        sdaLo();
    sclHi();
    sclLo();
    sdaHi();  // Release the data line for ACK from device
}

uint8_t software_i2c_t::read1byte()
{
    uint8_t value = 0;
    sclHi();
    if (_sda.get())
        value += 0x80;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x40;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x20;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x10;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x08;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x04;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x02;
    sclLo();
    sclHi();
    if (_sda.get())
        value += 0x01;
    sclLo();

    return value;
}

// return true on ACK
// Can also be used by controller to send NACK after last byte is read from device
bool software_i2c_t::checkAckBit()
{
    uint8_t ack;
    sdaHi();  // Release data line. This will cause a NACK from controller when reading bytes.
    sclHi();
    ack = _sda.get();
    sclLo();
    return !ack;
}

// Used by controller to ACK to device bewteen multi-byte reads
void software_i2c_t::writeAck()
{
    sdaLo();
    sclHi();
    sclLo();
    sdaHi();  // Release the data line
}

}  // namespace peripherals
