#pragma once

#include "stm32f4xx_hal.h"

namespace ZOQ::stm32_hal::stm32f4xx::uart {

class uart_it_t {
  public:
    using callback_t = void (*)(void* ctx, std::span<uint8_t> data);
    uart_it_t(UART_HandleTypeDef& huart) : _huart{huart} { HAL_UART_Receive_IT(&_huart, &_d, 1); }

    constexpr void set_callback(void* ctx, callback_t cb) { _ctx = ctx, _cb = cb; }

    void RxCpltCallback(UART_HandleTypeDef* h) {
        if (h != &_huart)
            return;
        if (_cb)
            _cb(_ctx, {&_d, 1});
        HAL_UART_Receive_IT(&_huart, &_d, 1);
    }

  private:
    UART_HandleTypeDef& _huart;
    void* _ctx{};
    callback_t _cb{};

    uint8_t _d{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::uart