#pragma once

#ifdef STM32F103xB

#include <cassert>
#include <span>

#include "itf/spi_itf.hpp"
#include "stm32f1xx_hal.h"

namespace stm32f1xx::spi {

class spi_t final : public itf::spi_itf {
  public:
    constexpr spi_t(SPI_HandleTypeDef& hspi) : _hspi(hspi){};

    // itf::spi_itf
    void write(const std::span<const uint8_t>& data) override
    {
        auto status = HAL_SPI_Transmit(&_hspi, const_cast<uint8_t*>(data.data()), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

    void read(const std::span<uint8_t>& data)
    {
        if (0 == data.size_bytes())
            return;
        auto status = HAL_SPI_Receive(&_hspi, data.data(), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

    void write(uint8_t byte)
    {
        auto status = HAL_SPI_Transmit(&_hspi, &byte, 1, HAL_MAX_DELAY);
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

}  // namespace stm32f4xx::spi

#endif
