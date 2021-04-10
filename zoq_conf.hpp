#pragma once


#if defined ( STM32F412Rx )
	#include "stm32f4xx_hal.h"
	#include "Stm32F4/pio_defs.hpp"
#elif defined ( STM32F1xx )
	#include "stm32f1xx_hal.h"
	#include "Stm32F1/pio_defs.hpp"
	#include "Stm32F1/hal_uart_dma.hpp"
	#include "Stm32F1/pio.hpp"
#elif defined (STM32F042x6)
	#include "stm32f0xx_hal.h"
	#include "pio_defs_hal.hpp"
#endif

