#pragma once
#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {
	class uart {
	public:
		inline uart(USART_TypeDef* instance, const UART_InitTypeDef& init);
		inline ~uart();
		inline HAL_StatusTypeDef transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
		inline HAL_StatusTypeDef receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);
	private:
		UART_HandleTypeDef handle;

	public:
		template<int uart_num>
		inline static void clk_enable();
		
		template<int uart_num>
		inline static bool is_clk_enabled();
		
		template<int uart_num>
		inline static void clk_disable();
		
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
	
	HAL_StatusTypeDef uart::receive(uint8_t *pData, uint16_t Size, uint32_t Timeout) {
		return HAL_UART_Receive(&handle, pData, Size, Timeout);
	}

	#ifdef USART1
		template<>
		inline void uart::clk_enable<1>() {
			__HAL_RCC_USART1_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<1>() {
			return __HAL_RCC_USART1_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<1>() {
			__HAL_RCC_USART1_CLK_DISABLE();
		}	
	#endif
	
	
	#ifdef USART2
		template<>
		inline void uart::clk_enable<2>() {
			__HAL_RCC_USART2_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<2>() {
			return __HAL_RCC_USART2_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<2>() {
			__HAL_RCC_USART2_CLK_DISABLE();
		}	
	#endif
		
	#ifdef USART3
		template<>
		inline void uart::clk_enable<3>() {
			__HAL_RCC_USART3_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<3>() {
			return __HAL_RCC_USART3_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<3>() {
			__HAL_RCC_USART3_CLK_DISABLE();
		}	
	#endif
	
	#ifdef USART4
		template<>
		inline void uart::clk_enable<4>() {
			__HAL_RCC_USART4_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<4>() {
			return __HAL_RCC_USART4_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<4>() {
			__HAL_RCC_USART4_CLK_DISABLE();
		}
	#endif
	#ifdef USART5
		template<>
		inline void uart::clk_enable<5>() {
			__HAL_RCC_USART5_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<5>() {
			return __HAL_RCC_USART5_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<5>() {
			__HAL_RCC_USART5_CLK_DISABLE();
		}
	#endif	
} // namespace