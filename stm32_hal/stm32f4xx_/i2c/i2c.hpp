#pragma once
#include <cassert>
#include <span>

#include "stm32f4xx_hal.h"

namespace stm32f4xx::i2c {

class i2c_t {
  public:
    constexpr i2c_t(I2C_HandleTypeDef& hi2c) : _hi2c(hi2c){};
    void write(const std::span<const uint8_t>& data, uint16_t addr)
    {
        auto status =
            HAL_I2C_Master_Transmit(&_hi2c, addr, const_cast<uint8_t*>(data.data()), data.size_bytes(), HAL_MAX_DELAY);
        assert(HAL_OK == status);
    };

  private:
    I2C_HandleTypeDef& _hi2c;
};

}  // namespace stm32f4xx::i2c
