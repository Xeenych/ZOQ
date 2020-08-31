#pragma once
#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {
	
	enum class pinState {
		Reset = GPIO_PIN_RESET,
		Set = GPIO_PIN_SET
	};
	
	inline GPIO_PinState convert(pinState state) noexcept {
		return (state == pinState::Set)? GPIO_PIN_SET : GPIO_PIN_RESET;
	}
	
}	// namespace