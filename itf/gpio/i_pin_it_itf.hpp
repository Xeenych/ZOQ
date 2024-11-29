#pragma once

namespace ZOQ::itf {

class i_pin_it_itf {
  public:
    using callback_t = void (*)(void* ctx);
    virtual bool get() = 0;
    virtual void set_callback(callback_t cb, void* ctx) = 0;
    virtual ~i_pin_it_itf() = default;
};

}  // namespace itf
