#pragma once
#include "pio_common.hpp"

namespace ZOQ::Stm32_HAL {

	class pio_output {
	public:
		inline pio_output ( Pin const& p,
						   pinState initial_state = pinState::Reset,
						   uint32_t Mode = GPIO_MODE_OUTPUT_PP,
						   uint32_t Pull = GPIO_NOPULL,
						   uint32_t Speed = GPIO_SPEED_FREQ_LOW) ;
		inline void set() const ;
		inline void reset() const ;
		inline void set(pinState state) const;
		inline pinState read() const ;
		inline ~pio_output() ;
	private:
		Pin const& pin;
	};

	pio_output::pio_output( Pin const& p, pinState initial_state, uint32_t Mode, uint32_t Pull, uint32_t Speed)
		: pin(p)
		{
			set(initial_state);

			GPIO_InitTypeDef GPIO_InitStruct =
			{
				.Pin = (uint32_t)(1<<pin.pin),
				.Mode = Mode,
				.Pull = Pull,
				.Speed = Speed
			};
			HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
		};

	pio_output::~pio_output() {
		HAL_GPIO_DeInit(pin.port, pin.pin);
	}

	void pio_output::reset() const {
		setPin(pin, pinState::Reset);
	}

	void pio_output::set() const  {
		setPin(pin, pinState::Set);
	}

	void pio_output::set(pinState state) const {
		setPin(pin, state);
	}

	pinState pio_output::read() const  {
		return getPinState(pin);
	}
}	// namespace