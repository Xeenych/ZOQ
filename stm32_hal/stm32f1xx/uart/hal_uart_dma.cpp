#if __has_include("stm32f1xx_hal.h")

#include "hal_uart_dma.hpp"

/*
            (+) HAL_UART_TxHalfCpltCallback()
        (+) HAL_UART_TxCpltCallback()
        (+) HAL_UART_RxHalfCpltCallback()
        (+) HAL_UART_RxCpltCallback()
        (+) HAL_UART_ErrorCallback()

        (+) HAL_UART_AbortCpltCallback()
        (+) HAL_UART_AbortTransmitCpltCallback()
        (+) HAL_UART_AbortReceiveCpltCallback()
*/

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    auto uart = hal_uart_dma::select_uart_by_handle(huart);
    if (uart != nullptr)
        uart->TxCpltCallback();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    auto uart = hal_uart_dma::select_uart_by_handle(huart);
    if (uart != nullptr)
        uart->RxCpltCallback();
}

#endif
