#pragma once
#include <stdint.h>

class systick_timer {
public:
	inline systick_timer(uint32_t timeout);
	inline bool elapsed() const;
	inline void reset();
	inline void set_timeout(uint32_t _timeout);
private:
	uint32_t timeout_tick;
	uint32_t timeout;
};

systick_timer::systick_timer(uint32_t _timeout)
	: timeout_tick(HAL_GetTick()), timeout(_timeout) {
}

bool systick_timer::elapsed() const {
	uint32_t delta = HAL_GetTick() - timeout_tick;
	if ( delta >= timeout )
		return true;
	return false;
}

void systick_timer::reset() {
	timeout_tick = HAL_GetTick();
}

void systick_timer::set_timeout(uint32_t _timeout) {
	timeout = _timeout;
}