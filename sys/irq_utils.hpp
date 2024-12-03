#pragma once

#include "ZOQ/stm32/stm32.hpp"
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

class priority_section_t {
  public:
    priority_section_t(uint32_t level) : _value{__get_BASEPRI()} { __set_BASEPRI_MAX(level << (8 - __NVIC_PRIO_BITS)); }
    ~priority_section_t() { __set_BASEPRI(_value); }

  private:
    const uint32_t _value;
};
