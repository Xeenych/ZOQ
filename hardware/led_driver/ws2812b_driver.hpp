#pragma once
#include <cassert>

#include "ZOQ/itf/spi/spi_itf.hpp"

namespace ZOQ::hardware::led_driver {

using namespace ZOQ::itf;

// Драйвер WS2812b
// Использует SPI
// Посылка 8 бит
// Частота SPI должна быть 3.33 МГц
template <size_t NUM_LEDS>
class ws2812b_t {
  public:
    constexpr ws2812b_t(spi_itf& spi) : _spi(spi){};
    constexpr size_t num_leds() const { return NUM_LEDS; }

    void set_led(uint8_t r, uint8_t g, uint8_t b, size_t led_n);
    void clear();
    void reset();
    void apply();

  private:
    // 00 - 0x88, 01 - 0x8e, 10 - 0xe8, 11 - 0xee
    static constexpr uint8_t _bit_table[] = {0x88, 0x8e, 0xe8, 0xee};

    spi_itf& _spi;

    uint8_t _out_data[12 * NUM_LEDS] = {};  // GRB
};

template <size_t NUM_LEDS>
void ws2812b_t<NUM_LEDS>::set_led(uint8_t r, uint8_t g, uint8_t b, size_t led_n)
{
    assert(led_n < NUM_LEDS);
    uint8_t* ptr = _out_data + led_n * 12;
    ptr[0] = _bit_table[(g >> 6) & 0x03];
    ptr[1] = _bit_table[(g >> 4) & 0x03];
    ptr[2] = _bit_table[(g >> 2) & 0x03];
    ptr[3] = _bit_table[(g >> 0) & 0x03];

    ptr[4] = _bit_table[(r >> 6) & 0x03];
    ptr[5] = _bit_table[(r >> 4) & 0x03];
    ptr[6] = _bit_table[(r >> 2) & 0x03];
    ptr[7] = _bit_table[(r >> 0) & 0x03];

    ptr[8] = _bit_table[(b >> 6) & 0x03];
    ptr[9] = _bit_table[(b >> 4) & 0x03];
    ptr[10] = _bit_table[(b >> 2) & 0x03];
    ptr[11] = _bit_table[(b >> 0) & 0x03];
}

template <size_t NUM_LEDS>
void ws2812b_t<NUM_LEDS>::clear()
{
    for (size_t led_idx = 0; led_idx < NUM_LEDS; led_idx++)
        set_led(0, 0, 0, led_idx);
}

template <size_t NUM_LEDS>
void ws2812b_t<NUM_LEDS>::reset()
{
    for (auto& i : _out_data)
        i = 0;
    apply();
}

template <size_t NUM_LEDS>
void ws2812b_t<NUM_LEDS>::apply()
{
    // data[12 * NUM_LEDS] = 0;
    _spi.write(_out_data);
}

}  // namespace ZOQ::hardware::led_driver
