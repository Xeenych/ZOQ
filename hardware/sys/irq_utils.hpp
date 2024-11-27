#pragma once

extern "C" {
#include "cmsis_compiler.h"
}

inline uint32_t save_interrupt()
{
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    return mask;
}

inline void restore_interrupt(uint32_t val)
{
    if (!val)
        __enable_irq();
}

class critical_section_t {
  public:
    critical_section_t() : _value{save_interrupt()} {}
    ~critical_section_t() { restore_interrupt(_value); }

  private:
    const uint32_t _value;
};
