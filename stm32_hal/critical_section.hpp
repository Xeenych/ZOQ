#pragma once
#include "cmsis_compiler.h"

namespace ZOQ {

class critical_section_t {
  public:
    critical_section_t() { __disable_irq(); }
    ~critical_section_t() { __enable_irq(); }
};

}  // namespace ZOQ
