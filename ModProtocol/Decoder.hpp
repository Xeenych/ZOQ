#pragma once
#include <cstdint>
#include <cstdlib>

#include "common.hpp"

namespace ZOQ::ModProtocol {

class Decoder {
  public:
    class IDecoderCallback {
      public:
        virtual void Execute(uint8_t const* buf, size_t len) = 0;
    };

    using callback_t = void (*)(uint8_t const* buf, size_t len);

  public:
    Decoder(uint8_t* buf, size_t size, IDecoderCallback& on_finish);
    void Decode(uint8_t const* chunk, size_t chunksize);  // возвращает количество отбработанных символов

  private:
    inline void reset();
    inline void decode_symbol(uint8_t byte);
    uint8_t* const buf_start;
    size_t const buf_size;
    uint8_t lrc = LRCSTART;
    bool is_high = true;
    uint8_t* decoded_ptr;
    IDecoderCallback& OnFinish;  // Колбэк, который вызывается при успешном декодировании посылки
    uint8_t current;
};

inline Decoder::Decoder(uint8_t* _buf, size_t _size, IDecoderCallback& on_finish)
    : buf_start(_buf), buf_size(_size), lrc(LRCSTART), is_high(true), decoded_ptr(_buf), OnFinish(on_finish)
{
}

inline void Decoder::decode_symbol(uint8_t symbol)
{
    if (symbol == FSTART) {
        reset();
        return;
    }

    if (symbol == FEND) {
        if (lrc == 0) {  // Приняли последний символ и сошлась контрольная сумма
            size_t decoded_len = decoded_ptr - buf_start - 1;  // длина посылки без контрольной суммы
            OnFinish.Execute(buf_start, decoded_len);
        }
        reset();
        return;
    }

    uint8_t decoded = CodeToByte(symbol);
    if (decoded == 0xff) {  // Приняли неизвестный символ
        reset();
        return;
    }

    if (is_high) {
        current = decoded << 4;
        is_high = false;
    } else {
        current = current | decoded;
        is_high = true;
        lrc -= current;
        if (decoded_ptr < buf_start + buf_size) {  // Если декодированная посылка помещается в буфер
            *decoded_ptr = current;
            decoded_ptr++;
        } else {
            reset();  // Декодированная посылка не умещается в буфер
        }
    }
}

inline void Decoder::Decode(uint8_t const* chunk, size_t chunksize)
{
    while (chunksize > 0) {
        decode_symbol(*chunk++);
        chunksize--;
    }
}

inline void Decoder::reset()
{
    decoded_ptr = buf_start;
    lrc = LRCSTART;
    is_high = true;
}

}  // namespace ZOQ::ModProtocol
