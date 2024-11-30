#pragma once
//#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {

	class uart {
	public:
		inline uart(USART_TypeDef* instance, const UART_InitTypeDef& init);
		inline ~uart();
		inline HAL_StatusTypeDef transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
		inline HAL_StatusTypeDef receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);
	private:
		UART_HandleTypeDef handle;
	}; // class
	
	uart::uart(USART_TypeDef* instance, const UART_InitTypeDef& init) {
		handle.Instance = instance;
		handle.Init = init;
		HAL_UART_Init(&handle);
	}
	
	uart::~uart() {
		HAL_UART_DeInit(&handle);
	}
	
	HAL_StatusTypeDef uart::transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout) {
		return HAL_UART_Transmit(&handle, pData, Size, Timeout);
	}
	
	HAL_StatusTypeDef uart::receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)  {
		return HAL_UART_Receive(&handle, pData, Size, Timeout);
	}
	
} // namespace