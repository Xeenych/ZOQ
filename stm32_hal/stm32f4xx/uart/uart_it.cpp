#include "uart_it.hpp"

#include <cassert>

namespace ZOQ::stm32_hal::stm32f4xx::uart {

uart_it_t::uart_it_t(UART_HandleTypeDef& h) : _h{h} { HAL_UART_Receive_IT(&_h, &_d, 1); }

void uart_it_t::write(const std::span<const uint8_t>& data) {
    auto status = HAL_UART_Transmit(&_h, data.data(), data.size_bytes(), HAL_MAX_DELAY);
    assert(HAL_OK == status);
}

void uart_it_t::OnRxCpltCallback(UART_HandleTypeDef* h) {
    if (h != &_h)
        return;
    HAL_UART_Receive_IT(&_h, &_d, 1);
    _cb.Execute({&_d, 1});
}

}  // namespace ZOQ::stm32_hal::stm32f4xx::uart
