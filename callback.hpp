#pragma once

namespace ZOQ {

class callback_itf {
  public:
    virtual void execute() = 0;
    virtual ~callback_itf() = default;
};

class callback_t {
  public:
    template <typename T, typename... Args>
    using fn_t = void (*)(T*, Args...);

    constexpr callback_t() = default;

    template <typename T, typename... Args>
    constexpr callback_t(const fn_t<T, Args...>& fn, void* arg) : _fn{reinterpret_cast<fn_t<void>>(fn)}, _arg{arg} {}

    template <typename... Args>
    constexpr void execute(Args... a) const {
        if (_fn) {
            auto f = reinterpret_cast<fn_t<void, Args...>>(_fn);
            f(_arg, a...);
        }
    }
    constexpr bool valid() const { return _fn; }
    constexpr void clear() { _fn = nullptr; }

  private:
    // function pointer to be called
    fn_t<void> _fn = nullptr;
    // function argument
    void* _arg = nullptr;
};

template <typename T>
struct callback_T : callback_itf {
    using fn_t = void (T::*)();
    inline callback_T(fn_t f, T* instance) : _f(f), _instance(instance) {};
    inline void execute() { (_instance->*_f)(); }
    fn_t _f;
    T* _instance;
};

}  // namespace  ZOQ
