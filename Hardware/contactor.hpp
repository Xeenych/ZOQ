#pragma once
#include "zoq.hpp"

using namespace ZOQ::Stm32_HAL;

namespace ZOQ::Hardware {

	using namespace ZOQ::Stm32f1xx_hal;

	class contactor {
	public:
		inline contactor(pio_output& _drv, pio_input& _status);
		inline void set() const;
		inline void reset() const;
		inline bool isSet() const;
	private:
		pio_output& driver_pin;
		pio_input& status_pin;
	};

	void contactor::set() const {
		driver_pin.set();
	}
	
	contactor::contactor(pio_output& _drv, pio_input& _status) : 
		driver_pin(_drv), status_pin(_status) {
    	contactor::reset();
	}
	
	void contactor::reset() const {
    	driver_pin.reset();
	}

	bool contactor::isSet() const {
		auto res = status_pin.read();
		return (res == pinState::Set)? true : false;
	}
} // namespace