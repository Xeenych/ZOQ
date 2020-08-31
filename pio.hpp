#pragma once
#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {

	class pioa final {
	public:
		pioa() = delete;
		~pioa() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOA_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOA_IS_CLK_ENABLED();
		}
	};
	
	class piob final {
	public:
		piob() = delete;
		~piob() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOB_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOB_IS_CLK_ENABLED();
		}
	};
	
	class pioc final {
	public:
		pioc() = delete;
		~pioc() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOC_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOC_IS_CLK_ENABLED();
		}
	};	
	
	class piod final {
	public:
		piod() = delete;
		~piod() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOD_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOD_IS_CLK_ENABLED();
		}
	};
	
	class pioe final {
	public:
		pioe() = delete;
		~pioe() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOE_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOE_IS_CLK_ENABLED();
		}
	};
	
	#ifdef GPIOF
	class piof final {
	public:
		piof() = delete;
		~piof() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOF_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOF_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOF_IS_CLK_ENABLED();
		}
	};	
	#endif
	
	#ifdef GPIOG
	class piog final {
	public:
		piog() = delete;
		~piog() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOG_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOG_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOG_IS_CLK_ENABLED();
		}
	};	
	#endif	

	#ifdef GPIOH
	class piog final {
	public:
		pioh() = delete;
		~pioh() = delete;
		static inline void clk_enable() {
			__HAL_RCC_GPIOH_CLK_ENABLE();
		}
		static inline void clk_disable() {
			__HAL_RCC_GPIOH_CLK_DISABLE();
		}
		static inline bool is_clk_enabled() {
			return __HAL_RCC_GPIOH_IS_CLK_ENABLED();
		}
	};	
	#endif	

} // namespace