#pragma once

namespace ZOQ::utils {

template <typename V = void*, typename... P>
using HandlerType = void (*)(P... args, V ctx);

template <typename... P>
class Callback {
  public:
    constexpr Callback() = default;

    template <typename V = void*>
    constexpr Callback(HandlerType<V, P...> h, void* ctx)
        : handler_{static_cast<HandlerType<void*, P...>>(h)}, ctx_{ctx} {}

    constexpr void Execute(P... args) const { handler_(args..., ctx_); }

  private:
    HandlerType<void*, P...> handler_{};  // функция обработчик колбека
    void* ctx_{};  // Контекст. Может использоваться для передачи в колбек дополнительного указателя
};

}  // namespace ZOQ::utils
