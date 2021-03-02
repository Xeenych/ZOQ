#include "debug_print.h"
#include "hal_uart_it.hpp"

static hal_uart_it* instance1 = nullptr;
static hal_uart_it* instance2 = nullptr;
static hal_uart_it* instance3 = nullptr;
static hal_uart_it* instance6 = nullptr;


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

	void hal_uart_it::enable_irq() {
		IRQn_Type irq = USART1_IRQn;
		
		switch ((uint32_t)(handle->Instance)) 
		{
		case (uint32_t)USART1:
			irq = USART1_IRQn;
			break;
		case (uint32_t)USART2:
			irq = USART2_IRQn;
			break;		
		case (uint32_t)USART3:
			irq = USART3_IRQn;
			break;		
		case (uint32_t)USART6:
			irq = USART6_IRQn;
			break;			
		default:
			return;
		}
		
		HAL_NVIC_EnableIRQ(irq);
	}
	
	void hal_uart_it::disable_irq() {
		IRQn_Type irq = USART1_IRQn;
		
		switch ((uint32_t)(handle->Instance)) 
		{
		case (uint32_t)USART1:
			irq = USART1_IRQn;
			break;
		case (uint32_t)USART2:
			irq = USART2_IRQn;
			break;		
		case (uint32_t)USART3:
			irq = USART3_IRQn;
			break;		
		case (uint32_t)USART6:
			irq = USART6_IRQn;
			break;			
		default:
			return;
		}		
		HAL_NVIC_DisableIRQ(irq);
	}
	
	
	hal_uart_it* hal_uart_it::select_instance(UART_HandleTypeDef const* h) {
		switch ((uint32_t)(h->Instance)) {
			case (uint32_t)USART1:
				return instance1;
			case (uint32_t)USART2:
				return instance2;
			case (uint32_t)USART3:
				return instance3;
			case (uint32_t)USART6:
				return instance6;				
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
			case (uint32_t)USART3:
				instance3 = this;
				break;	
			case (uint32_t)USART6:
				instance6 = this;
				break;				
				
		}
		auto status = HAL_UART_Receive_IT(handle, &tmp_rx, 1);
	}

	void hal_uart_it::OnTxCplt() {
		disable_irq();
		auto res = tx_buf.Pop(tmp_tx);
		enable_irq();
		if (res)
			HAL_UART_Transmit_IT(handle, &tmp_tx, 1);
	}

	void hal_uart_it::OnRxCplt() {
		disable_irq();
		rx_buf.Push(tmp_rx);
		enable_irq();
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
			disable_irq();
			auto res = tx_buf.Push(*pData);
			enable_irq();

			if ( !res )
				break;
			pData++;
			size--;
			count++;
		}


		// Проверяем идет ли передача
		disable_irq();
		auto state = HAL_UART_GetState(handle);
		enable_irq();

		if ( (state != HAL_UART_STATE_BUSY_TX)  && (state != HAL_UART_STATE_BUSY_TX_RX) ) {
			OnTxCplt(); // Предыдущая передача закончилась, надо начать новую
		}

		return count;
	}

	size_t hal_uart_it::read(void* _pData, size_t size) {

		uint32_t count = 0;
		uint8_t* pData = static_cast<uint8_t*>(_pData);

		while (size > 0) {
			disable_irq();
			auto res = rx_buf.Pop(*pData++);
			enable_irq();

			if (!res)
				break;

			pData++;
			size--;
			count++;
		}
		return count;
	}
}