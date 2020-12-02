#pragma once
#include <stdint.h>

class timeout_timer {
public:
	inline timeout_timer(uint32_t now, uint32_t timeout);
	inline bool elapsed(uint32_t now) const;
	inline void reset(uint32_t now);
	inline void set_timeout(uint32_t _timeout);
private:
	uint32_t timeout_tick;
	uint32_t timeout;
};

timeout_timer::timeout_timer(uint32_t now, uint32_t _timeout)
	: timeout_tick(now), timeout(_timeout) {
}

bool timeout_timer::elapsed(uint32_t now) const {
	uint32_t delta = now - timeout_tick;
	if ( delta >= timeout )
		return true;
	return false;
}

void timeout_timer::reset(uint32_t now) {
	timeout_tick = now;
}

void timeout_timer::set_timeout(uint32_t _timeout) {
	timeout = _timeout;
}