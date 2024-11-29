#pragma once
#include <cassert>
#include <span>

#include "ZOQ/itf/uart/uart_itf.hpp"
#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::uart {

class uart_t : public itf::uart_itf {
  public:
    constexpr uart_t(UART_HandleTypeDef& huart) : _huart(huart) {};
    void write(const std::span<const uint8_t>& data)
    {
        auto status = HAL_UART_Transmit(&_huart, const_cast<uint8_t*>(data.data()), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

    void read(const std::span<uint8_t>& data)
    {
        auto status = HAL_UART_Receive(&_huart, data.data(), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

  private:
    UART_HandleTypeDef& _huart;
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::uart
