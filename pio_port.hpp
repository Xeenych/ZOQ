#pragma once

#include "stm32f1xx_hal.h"

namespace ZOQ::Stm32f1xx_hal {

enum class portname {
	PA,
	PB,
	PC,
	PD
};

template <portname p>
class pio_port {
public:
	inline void CLK_ENABLE();
	inline void CLK_DISABLE();
};

template <>
inline void pio_port<portname::PA>::CLK_ENABLE() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

template <>
inline void pio_port<portname::PB>::CLK_ENABLE() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

template <>
inline void pio_port<portname::PC>::CLK_ENABLE() {
	__HAL_RCC_GPIOC_CLK_ENABLE();
}

template <>
inline void pio_port<portname::PD>::CLK_ENABLE() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

} // namespace