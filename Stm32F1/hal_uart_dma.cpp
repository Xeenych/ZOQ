#include "../zoq.hpp"
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

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	auto uart = hal_uart_dma::select_uart_by_handle(huart);
	if (uart!=nullptr)
		uart->TxCpltCallback();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	auto uart = hal_uart_dma::select_uart_by_handle(huart);
	if (uart!=nullptr)
		uart->RxCpltCallback();
}

namespace ZOQ::Stm32_HAL {
	hal_uart_dma* hal_uart_dma::uart1 = nullptr;
	hal_uart_dma* hal_uart_dma::uart2 = nullptr;
	hal_uart_dma* hal_uart_dma::uart3 = nullptr;
	hal_uart_dma* hal_uart_dma::uart4 = nullptr;
	hal_uart_dma* hal_uart_dma::uart5 = nullptr;
	hal_uart_dma* hal_uart_dma::uart6 = nullptr;
	hal_uart_dma* hal_uart_dma::uart7 = nullptr;
	
	hal_uart_dma* hal_uart_dma::select_uart_by_handle(UART_HandleTypeDef const* h) {
		switch ((uint32_t)h->Instance) {
		case (uint32_t)USART1_BASE:
			return hal_uart_dma::uart1;
		case (uint32_t)USART2_BASE:
			return hal_uart_dma::uart2;
		case (uint32_t)USART3_BASE:
			return hal_uart_dma::uart3;
		#ifdef USART4			
		case (uint32_t)USART4_BASE:
			return hal_uart_dma::uart4;
		#endif	
		#ifdef USART5			
		case (uint32_t)USART5_BASE:
			return hal_uart_dma::uart5;
		#endif				
		#ifdef USART6			
		case (uint32_t)USART6_BASE:
			return hal_uart_dma::uart6;
		#endif				
		#ifdef USART7			
		case (uint32_t)USART7_BASE:
			return hal_uart_dma::uart7;
		#endif	
		}
		return nullptr;
	}
}	// namespace
