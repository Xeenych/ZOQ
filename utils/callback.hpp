#pragma once

namespace ZOQ::utils {

template <typename V = void*, typename... P>
using HandlerType = void (*)(P... args, V ctx);

template <typename... P>
class Callback {
  public:
    template <typename V = void*>
    constexpr Callback(HandlerType<V> h, void* ctx) : handler_{reinterpret_cast<HandlerType<void*>>(h)}, ctx_{ctx} {}

    constexpr void Execute(P... args) const { handler_(args..., ctx_); }

    Callback(const Callback&) = delete;
    Callback& operator=(const Callback&) = delete;

  private:
    const HandlerType<void*> handler_;  // функция обработчик колбека
    void* const ctx_;  // Контекст. Может использоваться для передачи в колбек дополнительного указателя
};

}  // namespace ZOQ::utils
