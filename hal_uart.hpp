#pragma once

namespace ZOQ::Stm32_HAL {

	class hal_uart {
	public:
		hal_uart(UART_HandleTypeDef* h);
		size_t transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout); // возвращает количество переданных байт
		size_t receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);
	private:
		UART_HandleTypeDef* const handle;
	}; // class

	inline hal_uart::hal_uart(UART_HandleTypeDef* h) : handle(h) {
	}

	inline size_t hal_uart::transmit(uint8_t* pData, uint16_t Size, uint32_t Timeout) {
		auto res = HAL_UART_Transmit(handle, pData, Size, Timeout);
		if (res == HAL_ERROR || res == HAL_BUSY)
			return 0;
		
		if (res == HAL_TIMEOUT) {
			auto transferred = Size - (handle->TxXferCount + 1);
			return transferred;
		}
		
		return Size;
	}

	inline size_t hal_uart::receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)  {
		auto status = HAL_UART_Receive(handle, pData, Size, Timeout);
		if (status == HAL_ERROR || status == HAL_BUSY)
			return 0;
		
		if (status == HAL_TIMEOUT) {
			auto received = Size - (handle->RxXferCount + 1);
			return received;
		}
		
		return Size;
	}

} // namespace