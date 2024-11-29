#pragma once
#include <cassert>

#include "ZOQ/itf/spi/spi_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::spi {

using namespace ZOQ::itf;

class spi_t : public spi_itf {
  public:
    constexpr spi_t(SPI_HandleTypeDef& hspi) : _hspi(hspi) {};
    void write(const std::span<const uint8_t>& data) override
    {
        auto status = HAL_SPI_Transmit(&_hspi, const_cast<uint8_t*>(data.data()), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

    void read(const std::span<uint8_t>& data)
    {
        auto status = HAL_SPI_Receive(&_hspi, data.data(), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

    void write(uint8_t byte)
    {
        auto status = HAL_SPI_Transmit(&_hspi, &byte, 1, HAL_MAX_DELAY);
        assert(HAL_OK == status);
    }

    void readwrite(const std::span<uint8_t>& data, const std::span<uint8_t>& dataout) override
    {
        auto status = HAL_SPI_TransmitReceive(&_hspi, data.data(), dataout.data(), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    }

    uint8_t readwrite(uint8_t byte)
    {
        uint8_t rcv;
        auto status = HAL_SPI_TransmitReceive(&_hspi, &byte, &rcv, 1, HAL_MAX_DELAY);
        assert(HAL_OK == status);
        return rcv;
    }

  private:
    SPI_HandleTypeDef& _hspi;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::spi
