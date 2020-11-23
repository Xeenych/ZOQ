#pragma once
#include "zoq.hpp"


namespace ZOQ::Stm32_HAL {

	class hal_uart_it {
	public:
		hal_uart_it(UART_HandleTypeDef* h);
		uint16_t transmit(uint8_t *pData, uint16_t Size); // возвращает количество переданных байт
		uint16_t receive(uint8_t *pData, uint16_t Size);
		void flush();
	private:
		UART_HandleTypeDef* const handle;
		CyclicBufferT<uint8_t, 100> rx_buf;
		CyclicBufferT<uint8_t, 100> tx_buf;
		uint8_t tmp_rx;
		uint8_t tmp_tx;

		void OnRxCplt();
		void OnTxCplt();
		void OnError();


		friend void ::HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
		friend void ::HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
		friend void ::HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
	};
}