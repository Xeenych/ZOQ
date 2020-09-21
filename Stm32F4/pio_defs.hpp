#pragma once
#include "../pio_common.hpp"

namespace ZOQ::Stm32_HAL {

	enum class pinSpeed {
		Low = GPIO_SPEED_FREQ_LOW,
		Medium = GPIO_SPEED_FREQ_MEDIUM,
		High = GPIO_SPEED_FREQ_HIGH,
		VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH
	};
	
	inline pinSpeed getPinSpeed(Pin const& p) {
		auto ospeedr = p.port->OSPEEDR;
		auto bits = (ospeedr >> (p.pin_num << 1)) & 0x03;
		pinSpeed mode = static_cast<pinSpeed>(bits);
		return mode;
	}
	
	enum class pinMode { // MODER регистр
		Input = 0,
		Output = 1,
		Alternate = 2,
		Analog = 3
	};
	
	inline pinMode getPinMode(Pin const& p) {
		auto moder = p.port->MODER;
		auto bits = (moder >> (p.pin_num << 1)) & 0x03;
		pinMode mode = static_cast<pinMode>(bits);
		return mode;
	}
	
	inline pinState getPinState(Pin const& p) {
		auto idr = p.port->IDR;
		auto bit = idr & (0x01 << p.pin_num);
		return (bit != 0) ? pinState::Set : pinState::Reset;
	}
	
	inline void setPin(Pin const& p, pinState state) {
		if(state != pinState::Reset)
			p.port->BSRR = (1U<<p.pin_num);
  		else
  			p.port->BSRR = static_cast<uint32_t>(1U<<p.pin_num) << 16U;
	}
	
}	// namespace