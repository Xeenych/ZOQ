#pragma once

#include "cyclicbufferT.hpp"
#include "debug_print.h"
#include "zoq_conf.hpp"
#include "timeout_timer.hpp"
#include "IReadWrite.hpp"
#include "Bufreader.hpp"




#include "pio_output.hpp"
#include "pio_input.hpp"
#include "Hardware/contactor.hpp"


#ifdef HAL_CAN_MODULE_ENABLED
	#include "hal_can.hpp"
#endif

#if HAL_UART_MODULE_ENABLED
	#include "uart.hpp"
	#include "uartStream.hpp"
	#include "hal_uart.hpp"
	#include "hal_uart_it.hpp"
	#include "hal_uartT.hpp"
#endif


#include "ITextStream.hpp"
#include "rcc.hpp"

#include "ModProtocol\ModProtocol.hpp"
#include "ModProtocol\Encoder.hpp"
