#pragma once
#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {
	
	enum class pinState {
		Reset = GPIO_PIN_RESET,
		Set = GPIO_PIN_SET
	};
	
	inline GPIO_PinState convert(pinState state) {
		return (state == pinState::Set)? GPIO_PIN_SET : GPIO_PIN_RESET;
	}
	
	class pio {
	public:
		inline pio ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x);
		inline pio ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, pinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed);
		inline void set() const;
		inline void reset() const;
		inline pinState read() const;
		//inline static void GPIOA_CLK_ENABLE();
		//inline static void GPIOB_CLK_ENABLE();
		//inline static void GPIOC_CLK_ENABLE();
		//inline static void GPIOD_CLK_ENABLE();
	private:
		GPIO_TypeDef* const gpio_port;
		uint16_t const gpio_pin;
	};
	
	pio::pio( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, pinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed)
		:gpio_port(GPIOx), gpio_pin(GPIO_PIN_x) 
		{
			auto sss = convert(initial_state);
			HAL_GPIO_WritePin(gpio_port, gpio_pin, sss);
			GPIO_InitTypeDef GPIO_InitStruct = 
			{
				.Pin = GPIO_PIN_x,
				.Mode = Mode,
				.Pull = Pull,
				.Speed = Speed
			};
			HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
		};
		
	pio::pio( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x) 
		:gpio_port(GPIOx), gpio_pin(GPIO_PIN_x) 
		{	
			HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
			GPIO_InitTypeDef GPIO_InitStruct = 
			{
				.Pin = GPIO_PIN_x,
				.Mode = GPIO_MODE_OUTPUT_PP,
				.Pull = GPIO_NOPULL,
				.Speed = GPIO_SPEED_FREQ_LOW
			};
			HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
		}
	
	void pio::reset() const {
    	HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
	}

	void pio::set() const {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
	}
	
	pinState pio::read() const {
		auto res = HAL_GPIO_ReadPin(gpio_port, gpio_pin);
		return (res == GPIO_PIN_SET)? pinState::Set : pinState::Reset;
	}
}	// namespace