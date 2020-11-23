#include "debug_print.h"
#include "hal_uart_it.hpp"
#include "usart.h"

static hal_uart_it* instance2 = nullptr;
//static hal_uart_it* instance1 = nullptr;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	instance2->OnTxCplt();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	//instance1->OnRxCplt();
	instance2->OnRxCplt();
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	//instance1->OnError();
	instance2->OnError();
}

namespace ZOQ::Stm32_HAL {

	hal_uart_it::hal_uart_it(UART_HandleTypeDef* h) : handle(h) {
		if (h == &huart2)
			instance2 = this;
		auto status = HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}

	void hal_uart_it::OnTxCplt() {
		auto res = tx_buf.Pop(tmp_tx);
		if (res)
			HAL_UART_Transmit_IT(handle, &tmp_tx, 1);
	}

	void hal_uart_it::OnRxCplt() {
		rx_buf.Push(tmp_rx);
		HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}

	void hal_uart_it::OnError() {
		HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}


	uint16_t hal_uart_it::transmit(uint8_t *pData, uint16_t size) {

		if (size == 0)
			return size;

		uint32_t count = 0;
		while (!tx_buf.IsFull() && (size > 0)) {
			HAL_NVIC_DisableIRQ(USART2_IRQn);
			tx_buf.Push(*pData++);
			HAL_NVIC_EnableIRQ(USART2_IRQn);
			size--;
			count++;
		}
		OnTxCplt();
		return count;
	}

	uint16_t hal_uart_it::receive(uint8_t *pData, uint16_t size) {
		uint32_t count = 0;
		while (!rx_buf.IsEmpty() && (size > 0)) {
			HAL_NVIC_DisableIRQ(USART2_IRQn);
			rx_buf.Pop(*pData++);
			HAL_NVIC_EnableIRQ(USART2_IRQn);
			size--;
			count++;
		}

		return count;
	}
}