#pragma once

namespace ZOQ::Stm32f1xx_hal {

class pio_input {
public:
	inline pio_input ( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x);
	inline pinState read() const;
private:
	GPIO_TypeDef* const gpio_port;
	uint16_t const gpio_pin;
};

	pio_input::pio_input( GPIO_TypeDef* GPIOx, uint16_t GPIO_PIN_x) 
		:gpio_port(GPIOx), gpio_pin(GPIO_PIN_x) 
		{	
			GPIO_InitTypeDef GPIO_InitStruct = 
			{
				.Pin = GPIO_PIN_x,
				.Mode = GPIO_MODE_INPUT,
				.Pull = GPIO_NOPULL,
				.Speed = GPIO_SPEED_FREQ_LOW
			};
			HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
		}

	pinState pio_input::read() const {
		auto res = HAL_GPIO_ReadPin(gpio_port, gpio_pin);
		return (res == GPIO_PIN_SET)? pinState::Set : pinState::Reset;
	}

} // namespace