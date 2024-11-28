#pragma once

namespace ZOQ {

class callback_itf {
  public:
    virtual void execute() = 0;
    virtual ~callback_itf() = default;
};

class callback_t {
  public:
    template <typename T>
    using fn_t = void (*)(T*);

    constexpr callback_t() = default;

    template <typename T>
    constexpr callback_t(const fn_t<T>& fn, void* arg) : _fn(reinterpret_cast<fn_t<void>>(fn)), _arg(arg){};
    constexpr void execute() const
    {
        if (_fn)
            _fn(_arg);
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
