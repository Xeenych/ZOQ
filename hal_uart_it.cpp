#include "debug_print.h"
#include "hal_uart_it.hpp"
#include "usart.h"

static hal_uart_it* instance2 = nullptr;
static hal_uart_it* instance1 = nullptr;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	auto in = hal_uart_it::select_instance(huart);
	if (in == nullptr)
		return;
	in->OnTxCplt();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	auto in = hal_uart_it::select_instance(huart);
	if (in == nullptr)
		return;
	in->OnRxCplt();
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	auto in = hal_uart_it::select_instance(huart);
	if (in == nullptr)
		return;
	in->OnError();
}

namespace ZOQ::Stm32_HAL {

	hal_uart_it* hal_uart_it::select_instance(UART_HandleTypeDef const* h) {
		switch ((uint32_t)(h->Instance)) {
			case (uint32_t)USART1:
				return instance1;
			case (uint32_t)USART2:
				return instance2;
			default:
				return nullptr;
		}
	}

	hal_uart_it::hal_uart_it(UART_HandleTypeDef* h) : handle(h) {
		switch ((uint32_t)(h->Instance)) {
			case (uint32_t)USART1:
				instance1 = this;
				break;
			case (uint32_t)USART2:
				instance2 = this;
				break;
		}
		auto status = HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}

	void hal_uart_it::OnTxCplt() {
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		auto res = tx_buf.Pop(tmp_tx);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		if (res)
			HAL_UART_Transmit_IT(handle, &tmp_tx, 1);
	}

	void hal_uart_it::OnRxCplt() {
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		rx_buf.Push(tmp_rx);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}

	void hal_uart_it::OnError() {
		HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}


	size_t hal_uart_it::write(void *_pData, size_t size) {

		if (size == 0)
			return size;

		uint32_t count = 0;
		uint8_t* pData = static_cast<uint8_t*>(_pData);

		while (size > 0) {
			HAL_NVIC_DisableIRQ(USART2_IRQn);
			auto res = tx_buf.Push(*pData);
			HAL_NVIC_EnableIRQ(USART2_IRQn);

			if ( !res )
				break;
			pData++;
			size--;
			count++;
		}


		// Проверяем идет ли передача
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		auto state = HAL_UART_GetState(handle);
		HAL_NVIC_EnableIRQ(USART2_IRQn);

		if ( (state != HAL_UART_STATE_BUSY_TX)  && (state != HAL_UART_STATE_BUSY_TX_RX) ) {
			OnTxCplt(); // Предыдущая передача закончилась, надо начать новую
		}

		return count;
	}

	size_t hal_uart_it::read(void* _pData, size_t size) {

		uint32_t count = 0;
		uint8_t* pData = static_cast<uint8_t*>(_pData);

		while (size > 0) {
			HAL_NVIC_DisableIRQ(USART2_IRQn);
			auto res = rx_buf.Pop(*pData++);
			HAL_NVIC_EnableIRQ(USART2_IRQn);

			if (!res)
				break;

			pData++;
			size--;
			count++;
		}


		return count;
	}
}