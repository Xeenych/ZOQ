#pragma once

namespace ZOQ {

class callback_itf {
  public:
    virtual void execute() = 0;
    virtual ~callback_itf() = default;
};

class callback_t {
  public:
    using fn_t = void (*)(void*);
    constexpr callback_t() = default;
    constexpr callback_t(const fn_t& fn, void* arg) : _fn(fn), _arg(arg) {}
    constexpr void execute() const { _fn(_arg); }
    constexpr bool valid() const { return _fn; }
    constexpr void clear() { _fn = nullptr; }

  private:
    // function pointer to be called
    fn_t _fn = nullptr;
    // function argument
    void* _arg = nullptr;
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
