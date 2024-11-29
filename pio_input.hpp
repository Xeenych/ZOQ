#pragma once
#include "pio_common.hpp"

namespace ZOQ::Stm32_HAL {

class pio_input {
  public:
    inline pio_input(Pin const& p, uint32_t Pull = GPIO_NOPULL, uint32_t Mode = GPIO_MODE_INPUT);
    inline pinState read() const;
    inline ~pio_input();

  private:
    Pin const& pin;
};

pio_input::pio_input(Pin const& p, uint32_t Pull, uint32_t Mode) : pin(p)
{
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = (uint32_t)(1 << pin.pin_num), .Mode = Mode, .Pull = Pull, .Speed = GPIO_SPEED_FREQ_LOW};
    HAL_GPIO_Init(pin.port, &GPIO_InitStruct);
}

pio_input::~pio_input()
{
    HAL_GPIO_DeInit(pin.port, pin.pin_num);
}

pinState pio_input::read() const
{
    return getPinState(pin);
}

}  // namespace ZOQ::Stm32_HAL