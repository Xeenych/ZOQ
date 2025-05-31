#pragma once
#include <cassert>

#include "ZOQ/itf/spi/spi_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::spi {

using ZOQ::itf::spi_itf;

class spi_dma_t : public spi_itf {
  public:
    constexpr explicit spi_dma_t(SPI_HandleTypeDef& hspi) : _hspi(hspi) {}
    spi_dma_t(const spi_dma_t&) = delete;
    spi_dma_t& operator=(const spi_dma_t&) = delete;

    void write(const std::span<const uint8_t>& data) override {
        auto status = HAL_SPI_Transmit_DMA(&_hspi, data.data(), data.size_bytes());
        assert(HAL_OK == status);
    }

    void read(const std::span<uint8_t>& data) override {
        auto status = HAL_SPI_Receive_DMA(&_hspi, data.data(), data.size_bytes());
        assert(HAL_OK == status);
    }

    void write(uint8_t byte) {
        auto status = HAL_SPI_Transmit(&_hspi, &byte, 1, HAL_MAX_DELAY);
        assert(HAL_OK == status);
    }

    void readwrite(const std::span<uint8_t>& data, const std::span<uint8_t>& dataout) override {
        auto status = HAL_SPI_TransmitReceive_DMA(&_hspi, data.data(), dataout.data(), data.size_bytes());
        assert(HAL_OK == status);
    }

    uint8_t readwrite(uint8_t byte) override {
        uint8_t rcv = 0;
        auto status = HAL_SPI_TransmitReceive_DMA(&_hspi, &byte, &rcv, 1);
        assert(HAL_OK == status);
        return rcv;
    }

  private:
    SPI_HandleTypeDef& _hspi;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::spi
