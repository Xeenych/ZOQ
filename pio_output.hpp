#pragma once
#include "pio_defs.hpp"

namespace ZOQ::Stm32f1xx_hal {

	class pio_output {
	public:
		inline pio_output ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x) ;
		inline pio_output ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, pinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed) ;
		inline void set() const ;
		inline void reset() const ;
		inline pinState read() const ;
		inline ~pio_output() ;
	private:
		GPIO_TypeDef* const gpio_port;
		uint16_t const gpio_pin;
	};
	
	pio_output::pio_output( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x, pinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed) 
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
		
	pio_output::~pio_output() {
		HAL_GPIO_DeInit(gpio_port, gpio_pin);
	}
		
	pio_output::pio_output( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x)
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
	
	void pio_output::reset() const {
    	HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
	}

	void pio_output::set() const  {
		HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
	}
	
	pinState pio_output::read() const  {
		auto res = HAL_GPIO_ReadPin(gpio_port, gpio_pin);
		return (res == GPIO_PIN_SET)? pinState::Set : pinState::Reset;
	}
}	// namespace