#pragma once
#include "../pio_common.hpp"

namespace ZOQ::Stm32_HAL {

	enum class pinSpeed : uint32_t {
		Low = GPIO_SPEED_FREQ_LOW,
		Medium = GPIO_SPEED_FREQ_MEDIUM,
		High = GPIO_SPEED_FREQ_HIGH,
	};

	inline void setPin(Pin const& p, pinState state) {
		if(state != pinState::Reset)
			p.port->BSRR = (1U << p.pin_num);
  		else
  			p.port->BSRR = static_cast<uint32_t>(1U<<p.pin_num) << 16U;
	}

	inline pinState getPinState(Pin const& p) {
		auto idr = p.port->IDR;
		auto bit = idr & (0x01 << p.pin_num);
		return (bit != 0) ? pinState::Set : pinState::Reset;
	}

}	// namespace