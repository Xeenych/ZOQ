#pragma once

namespace ZOQ {

class callback_itf {
  public:
    virtual void execute() = 0;
    virtual ~callback_itf() = default;
};

struct callback_t {
    inline void execute() const { fn(arg); }
    void (*fn)(void* arg);
    void* arg;
};

template <typename T>
struct callback_T : callback_itf {
    using fn_t = void (T::*)();
    inline callback_T(fn_t f, T* instance) : _f(f), _instance(instance){};
    inline void execute() { (_instance->*_f)(); }
    fn_t _f;
    T* _instance;
};

}  // namespace  ZOQ
