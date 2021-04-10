#pragma once
#include "pio_common.hpp"

namespace ZOQ::Stm32_HAL {

	enum class pinSpeed : uint32_t {
		Low = GPIO_SPEED_FREQ_LOW,
		Medium = GPIO_SPEED_FREQ_MEDIUM,
		High = GPIO_SPEED_FREQ_HIGH,
	};

	inline void setPin(Pin const& p, pinState state) {
		HAL_GPIO_WritePin(p.port, 1U << p.pin_num, GPIO_PIN_SET);
	}

	inline pinState getPinState(Pin const& p) {
		auto result = HAL_GPIO_ReadPin(p.port, 1U << p.pin_num);
		if (result == GPIO_PIN_RESET)
			return pinState::Reset;
		else
			return pinState::Set;
	}

}	// namespace