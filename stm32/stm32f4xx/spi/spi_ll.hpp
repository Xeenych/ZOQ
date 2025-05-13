#pragma once
#include "itf/spi_itf.hpp"
#include "stm32f4xx_ll_spi.h"

namespace stm32f4xx::spi {

// LL драйвер SPI
// Похоже, что вполне работосопсобен
// Очень быстрый
class spi_ll_t final : public itf::spi_itf {
  public:
    constexpr explicit spi_ll_t(SPI_TypeDef* spix) : _hspi{spix} { LL_SPI_Enable(_hspi); }
    ~spi_ll_t() override { LL_SPI_Disable(_hspi); }

    spi_ll_t(const spi_ll_t&) = delete;
    spi_ll_t& operator=(const spi_ll_t&) = delete;

    void write(const std::span<const uint8_t>& data) override {
        for (auto d : data) {
            write8(d);
            read8();
        }
    }

    void read(const std::span<uint8_t>& data) override {
        for (auto& d : data) {
            write8(0);  // To read something we must transmit something
            d = read8();
        }
    }

    uint8_t readwrite(uint8_t byte) override {
        write8(byte);
        return read8();
    }

  private:
    SPI_TypeDef* const _hspi;

    void write8(uint8_t byte) {
        while (!LL_SPI_IsActiveFlag_TXE(_hspi)) {
            // busy wait
        }
        LL_SPI_TransmitData8(_hspi, byte);
    }

    uint8_t read8() {
        while (!LL_SPI_IsActiveFlag_RXNE(_hspi)) {
            // busy wait
        }
        return LL_SPI_ReceiveData8(_hspi);
    }
};

}  // namespace stm32f4xx::spi
