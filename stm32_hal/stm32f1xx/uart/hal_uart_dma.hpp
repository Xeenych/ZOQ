#pragma once

#if __has_include("stm32f1xx_hal.h")
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32_HAL {
class hal_uart_dma {
  public:
    hal_uart_dma(UART_HandleTypeDef* h, uint8_t* rxbuffer, size_t rxbuffer_size, uint8_t* txbuffer,
                 size_t txbuffer_size);
    size_t transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout);
    size_t receive(uint8_t* pData, uint16_t Size, uint32_t Timeout);
    size_t bytes_available() const;
    void flush();

  private:
    UART_HandleTypeDef* const handle;
    uint8_t* const rxbuffer;
    size_t const rxbuffer_size;
    uint8_t* const txbuffer;
    size_t const txbuffer_size;
    uint8_t* read_ptr;
    static hal_uart_dma* uart1;
    static hal_uart_dma* uart2;
    static hal_uart_dma* uart3;
    static hal_uart_dma* uart4;
    static hal_uart_dma* uart5;
    static hal_uart_dma* uart6;
    static hal_uart_dma* uart7;
    static hal_uart_dma* select_uart_by_handle(UART_HandleTypeDef const* h);

    void TxCpltCallback();
    void RxCpltCallback();

    friend void ::HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);
    friend void ::HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
};
// class

inline void hal_uart_dma::TxCpltCallback() {}

inline void hal_uart_dma::RxCpltCallback()
{
    auto result = HAL_UART_Receive_DMA(handle, rxbuffer, rxbuffer_size);
    assert(result == HAL_OK);  // define NDEBUG to remove this assert
}

inline hal_uart_dma::hal_uart_dma(UART_HandleTypeDef* h, uint8_t* _rxbuffer, size_t _rxbuffer_size, uint8_t* _txbuffer,
                                  size_t _txbuffer_size)
    : handle(h),
      rxbuffer(_rxbuffer),
      rxbuffer_size(_rxbuffer_size),
      txbuffer(_txbuffer),
      txbuffer_size(_txbuffer_size),
      read_ptr(_rxbuffer)
{
    auto result = HAL_UART_Receive_DMA(handle, rxbuffer, rxbuffer_size);
    assert(result == HAL_OK);  // define NDEBUG to remove this assert
}

inline size_t hal_uart_dma::transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();
    while (handle->gState != HAL_UART_STATE_READY) {
        if (Timeout != HAL_MAX_DELAY)
            if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
                return 0;
    }

    size_t to_copy = (Size > txbuffer_size) ? txbuffer_size : Size;
    memcpy(txbuffer, pData, to_copy);

    auto res = HAL_UART_Transmit_DMA(handle, txbuffer, to_copy);
    assert(res == HAL_OK);
    return to_copy;
}

inline size_t hal_uart_dma::bytes_available() const
{
    uint8_t* write_ptr = rxbuffer + rxbuffer_size - __HAL_DMA_GET_COUNTER(handle->hdmarx);

    if (write_ptr >= read_ptr) {
        size_t bytes_available = write_ptr - read_ptr;
        return bytes_available;
    } else {
        size_t bytes_available = write_ptr - read_ptr + rxbuffer_size;
        return bytes_available;
    }
}

inline size_t hal_uart_dma::receive(uint8_t* pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();

    size_t bytes_copied = 0;

    while (bytes_copied < Size) {
        if (Timeout != HAL_MAX_DELAY) {
            if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
                return bytes_copied;
        }

        uint8_t* write_ptr = rxbuffer + rxbuffer_size - __HAL_DMA_GET_COUNTER(handle->hdmarx);
        size_t bytes_available = (write_ptr >= read_ptr) ? write_ptr - read_ptr : rxbuffer + rxbuffer_size - read_ptr;

        size_t bytes_left_to_read = Size - bytes_copied;
        size_t bytes_to_copy = (bytes_available > bytes_left_to_read) ? bytes_left_to_read : bytes_available;
        uint8_t* dst = pData + bytes_copied;
        memcpy(dst, read_ptr, bytes_to_copy);
        bytes_copied += bytes_to_copy;
        read_ptr += bytes_to_copy;
        assert(read_ptr <= rxbuffer + rxbuffer_size);
        if (read_ptr == rxbuffer + rxbuffer_size)
            read_ptr = rxbuffer;
    }
    return bytes_copied;
}

inline void hal_uart_dma::flush()
{
    HAL_UART_AbortReceive(handle);
    handle->hdmarx->Instance->CNDTR = rxbuffer_size;
    read_ptr = rxbuffer;
    auto available = bytes_available();
    assert(available == 0);

    auto result = HAL_UART_Receive_DMA(handle, rxbuffer, rxbuffer_size);
    assert(result == HAL_OK);  // define NDEBUG to remove this assert
}
}  // namespace ZOQ::Stm32_HAL

#endif
