#pragma once
//#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32_HAL {

	template<UART_HandleTypeDef& handle>
	class hal_uartT {
	public:
		inline size_t transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
		inline size_t receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);
	};
	
	template<UART_HandleTypeDef& handle>
	size_t hal_uartT<handle>::transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout) {
		auto res = HAL_UART_Transmit(&handle, pData, Size, Timeout);
		if (res == HAL_ERROR || res == HAL_BUSY)
			return 0;
		
		if (res == HAL_TIMEOUT) {
			auto transferred = Size - handle.TxXferCount + 1;
			return transferred;
		}
		return Size;
	}
	
	template<UART_HandleTypeDef& handle>
	size_t hal_uartT<handle>::receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)  {
		auto status = HAL_UART_Receive(&handle, pData, Size, Timeout);
		if (status == HAL_ERROR || status == HAL_BUSY)
			return 0;
		
		if (status == HAL_TIMEOUT) {
			auto received = Size - (handle.RxXferCount + 1);
			return received;
		}
		
		return Size;
	}
	
} // namespace