#pragma once


#include "zoq_conf.hpp"

#if defined ( STM32F412Rx )
	#include "stm32f4xx_hal.h"
	#include "Stm32F4/pio_defs.hpp"
#elif defined ( STM32F105xC )
	#include "stm32f1xx_hal.h"
	#include "Stm32F1/pio_defs.hpp"
#endif



#include "pio.hpp"
#include "pio_output.hpp"
#include "pio_input.hpp"
#include "Hardware/contactor.hpp"
#include "hal_uart.hpp"
#include "hal_uart_dma.hpp"
#include "hal_uartT.hpp"

#ifdef HAL_CAN_MODULE_ENABLED
	#include "hal_can.hpp"
#endif

#include "uart.hpp"
#include "uartStream.hpp"
#include "ITextStream.hpp"
#include "rcc.hpp"

#include "ModProtocol\ModProtocol.hpp"
#include "ModProtocol\Encoder.hpp"

#include "cyclicbufferT.hpp"