#pragma once
#include <cstdint>

namespace ZOQ::itf {

struct pin_def_t {
    void* port;
    uint32_t pin;
};

class io_pin_itf {
  public:
    virtual void set() = 0;
    virtual void reset() = 0;
    virtual bool get() = 0;
    virtual ~io_pin_itf() = default;
};

}  // namespace ZOQ::itf
