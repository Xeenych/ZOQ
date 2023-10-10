#define LOG_ENABLE 1
#include "exti_interrupt_handler.hpp"

#include <cassert>

#include "logging.h"
#include "stm32f4xx.h"

struct handler_t {
    uint32_t pin;
    ZOQ::callback_t callback;
};

static handler_t exti_handlers[10]{};

extern "C" void EXTI9_5_IRQHandler(void)
{
    LOG_DBG("EXTI9_5_IRQHandler(void)");
    for (const auto& h : exti_handlers) {
        auto pin = h.pin;
        /* EXTI line interrupt detected */
        if (RESET != __HAL_GPIO_EXTI_GET_IT(pin)) {
            __HAL_GPIO_EXTI_CLEAR_IT(pin);
            h.callback.execute();
        }
    }
}

extern "C" void EXTI15_10_IRQHandler(void)
{
    for (const auto& h : exti_handlers) {
        auto pin = h.pin;
        /* EXTI line interrupt detected */
        if (RESET != __HAL_GPIO_EXTI_GET_IT(pin)) {
            __HAL_GPIO_EXTI_CLEAR_IT(pin);
            h.callback.execute();
        }
    }
}

namespace ZOQ::stm32::stm32f4xx::gpio {

void register_handler(uint32_t pin, const callback_t& cb)
{
    for (auto& h : exti_handlers)
        if (!h.callback.valid()) {
            h = {pin, cb};
            return;
        }
    assert(false);
}

}  // namespace ZOQ::stm32::stm32f4xx::gpio
