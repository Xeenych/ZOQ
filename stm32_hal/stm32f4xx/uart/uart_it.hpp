#pragma once

#include "stm32f4xx_hal.h"
#include <span>
#include <cassert>
#include "ZOQ/utils/callback.hpp"

namespace ZOQ::stm32_hal::stm32f4xx::uart {

class uart_it_t {
  public:
    // using callback_t = void (*)(void* ctx, std::span<uint8_t> data);
    using callback_t = ZOQ::utils::Callback<std::span<uint8_t>>;

    explicit uart_it_t(UART_HandleTypeDef& h);
    uart_it_t(const uart_it_t&) = delete;
    uart_it_t& operator=(const uart_it_t&) = delete;

    void write(const std::span<const uint8_t>& data);

    constexpr void set_callback(callback_t cb) { _cb = cb; }

    // Этот метод надо вызывать из прерывания
    void OnRxCpltCallback(UART_HandleTypeDef* h);

  private:
    UART_HandleTypeDef& _h;
    callback_t _cb{};
    uint8_t _d{};
};

}  // namespace ZOQ::stm32_hal::stm32f4xx::uart
