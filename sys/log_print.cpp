#include "usart.h"
#include <cassert>
#include <cstdarg>
#include <cstdio>
// #include <cstdlib>
#include "irq_utils.hpp"

void send_func(const char* pData, uint16_t Size)
{
    critical_section_t sec;
    HAL_GPIO_WritePin(USART2_RTS_GPIO_Port, USART2_RTS_Pin, GPIO_PIN_SET);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*)pData, Size, 20);
    // assert(HAL_OK == status);
}

void log_print(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    static char buf[280];
    const size_t size = sizeof(buf);
    {
        int len = vsnprintf(buf, size, fmt, ap);
        if ((len < 0) || ((size_t)len >= size))
            len = size;
        send_func(buf, len);
    }
    va_end(ap);
}
