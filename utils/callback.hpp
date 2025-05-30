#pragma once

namespace ZOQ::utils {

template <typename... P>
class Callback {
    template <typename V = void*>
    using HandlerType = void (*)(P... args, V ctx);

  public:
    constexpr Callback() = default;

    template <typename V = void*>
    constexpr Callback(HandlerType<V> h, V ctx) : handler_{reinterpret_cast<HandlerType<void*>>(h)}, ctx_{ctx} {}

    constexpr void Execute(P... args) const {
        if (handler_)
            handler_(args..., ctx_);
    }

  private:
    HandlerType<void*> handler_{};  // функция обработчик колбека
    void* ctx_{};  // Контекст. Может использоваться для передачи в колбек дополнительного указателя
};

}  // namespace ZOQ::utils
