#pragma once
#include "zoq.hpp"


namespace ZOQ::Stm32_HAL {

	class hal_uart_it : public IReadWrite {
	public:
		hal_uart_it(UART_HandleTypeDef* h);
		size_t write(void *pData, size_t size) override; // возвращает количество переданных байт
		size_t read(void *pData, size_t size) override;
	private:
		UART_HandleTypeDef* const handle;
		CyclicBufferT<uint8_t, 100> rx_buf;
		CyclicBufferT<uint8_t, 100> tx_buf;
		uint8_t tmp_rx;
		uint8_t tmp_tx;

		static hal_uart_it* select_instance(UART_HandleTypeDef const* h);

		void OnRxCplt();
		void OnTxCplt();
		void OnError();
		void enable_irq();
		void disable_irq();

		friend void ::HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
		friend void ::HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
		friend void ::HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
	};
}