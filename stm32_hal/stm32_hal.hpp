#pragma once

#if defined(STM32F0XX)
#include "stm32f0_hal.h"
#elseif defined(STM32F1XX)
#include "stm32f1_hal.h"
#elseif definef(STM32F4XX)
#include "stm32f4_hal.h"
#else
#endif