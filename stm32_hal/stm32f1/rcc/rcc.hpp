#pragma once
namespace ZOQ::Stm32f1xx_hal::rcc {
	
	namespace uart {
		template<int uart_num>
		inline static void clk_enable();
		
		template<int uart_num>
		inline static bool is_clk_enabled();
		
		template<int uart_num>
		inline static void clk_disable();
		
		#ifdef USART1
		template<>
		inline void uart::clk_enable<1>()  {
			__HAL_RCC_USART1_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<1>()  {
			return __HAL_RCC_USART1_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<1>()  { 
			__HAL_RCC_USART1_CLK_DISABLE();
		}	
	#endif
	
	
	#ifdef USART2
		template<>
		inline void uart::clk_enable<2>()  {
			__HAL_RCC_USART2_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<2>() {
			return __HAL_RCC_USART2_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<2>()  {
			__HAL_RCC_USART2_CLK_DISABLE();
		}	
	#endif
		
	#ifdef USART3
		template<>
		inline void uart::clk_enable<3>()  {
			__HAL_RCC_USART3_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<3>()  {
			return __HAL_RCC_USART3_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<3>()  {
			__HAL_RCC_USART3_CLK_DISABLE();
		}	
	#endif
	
	#ifdef USART4
		template<>
		inline void uart::clk_enable<4>()  {
			__HAL_RCC_USART4_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<4>()  {
			return __HAL_RCC_USART4_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<4>()  {
			__HAL_RCC_USART4_CLK_DISABLE();
		}
	#endif
	#ifdef USART5
		template<>
		inline void uart::clk_enable<5>()  {
			__HAL_RCC_USART5_CLK_ENABLE();
		}
	
		template<>
		inline bool uart::is_clk_enabled<5>()  {
			return __HAL_RCC_USART5_IS_CLK_ENABLED();
		}
	
		template<>
		inline void uart::clk_disable<5>()  {
			__HAL_RCC_USART5_CLK_DISABLE();
		}
	#endif

	}	// namespace uart
}	// namespace ZOQ::Stm32f1xx_hal::rcc
