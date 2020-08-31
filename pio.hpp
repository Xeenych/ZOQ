#pragma once
#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {
	
	class pio {
	public:
		inline pio ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, GPIO_PinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed);
		inline pio ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x);
		inline void set() const;
		inline void reset() const;
	private:
		GPIO_TypeDef* const gpio_port;
		uint16_t const gpio_pin;
	};
	
	pio::pio( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, GPIO_PinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed)
		:gpio_port(GPIOx), gpio_pin(GPIO_PIN_x) 
		{
			HAL_GPIO_WritePin(gpio_port, gpio_pin, initial_state);
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

}	// namespace