#pragma once
#include "drv/itf/o_pin_itf.hpp"
#include "drv/itf/spi_itf.hpp"

namespace peripherals {

using namespace itf;

// Драйвер mcp23s17
// Работаем в режиме IOCON.BANK=0, IOCON.HAEN=0
class mcp23s17_t {
  public:
    enum class port { porta, portb };
    enum class dir : uint8_t { output = 0, input = 1 };

    constexpr mcp23s17_t(spi_itf& spi, o_pin_itf& cs) : _spi{spi}, _cs{cs} {}
    mcp23s17_t(const mcp23s17_t&) = delete;
    mcp23s17_t& operator=(const mcp23s17_t&) = delete;

    // Установить направление пина
    void set_dir(uint8_t pin, port p, dir d) { write_port_reg(pin, p, reg_iodira, reg_iodirb, static_cast<bool>(d)); }
    // Выставить значение пина в регистре GPIOx
    void set_output(uint8_t pin, port p, bool level) { write_port_reg(pin, p, reg_gpioa, reg_gpiob, level); }
    // Прочитать значение пина в регистре GPIOx
    bool get_output(uint8_t pin, port p) { return read_port_reg(pin, p, reg_gpioa, reg_gpiob); }
    // Выставить значение пина в защелке OLATx
    void set_output_latch(uint8_t pin, port p, bool level) { write_port_reg(pin, p, reg_olata, reg_olatb, level); }
    // Прочитать значение пина в регистре OLATx
    bool get_output_latch(uint8_t pin, port p) { return read_port_reg(pin, p, reg_olata, reg_olatb); }

  private:
    spi_itf& _spi;
    o_pin_itf& _cs;

    static constexpr uint8_t reg_iodira = 0x00;
    static constexpr uint8_t reg_iodirb = 0x01;
    static constexpr uint8_t reg_gpioa = 0x12;
    static constexpr uint8_t reg_gpiob = 0x13;
    static constexpr uint8_t reg_olata = 0x14;
    static constexpr uint8_t reg_olatb = 0x15;

    // Запись регистра по SPI
    void write_reg(uint8_t addr, uint8_t value)
    {
        uint8_t cmd[] = {0x40, addr, value};
        _cs.reset();
        _spi.write(cmd);
        _cs.set();
    }

    // Чтение решистра по SPI
    uint8_t read_reg(uint8_t addr)
    {
        uint8_t cmd[] = {0x41, addr, 0x00};
        uint8_t rd[3]{};
        _cs.reset();
        _spi.readwrite(cmd, rd);
        _cs.set();
        return rd[2];
    }

    // Функция-помощник для записи регистра порта
    void write_port_reg(uint8_t pin, port p, uint8_t rega, uint8_t regb, bool v)
    {
        uint8_t reg = (port::porta == p) ? rega : regb;
        uint8_t val = read_reg(reg);
        uint8_t newval = v ? val | (1 << pin) : val & (~(1 << pin));
        write_reg(reg, newval);
    }

    // Функция-помощник для чтения регистра порта
    bool read_port_reg(uint8_t pin, port p, uint8_t rega, uint8_t regb)
    {
        uint8_t reg = (port::porta == p) ? rega : regb;
        uint8_t val = read_reg(reg);
        return val & (1 << pin);
    }
};

}  // namespace peripherals
