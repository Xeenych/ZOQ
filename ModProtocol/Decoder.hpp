#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "common.hpp"

namespace ZOQ::ModProtocol {


class Decoder {

using callback_t = void (*)(uint8_t const* buf, size_t len);

public:
	Decoder(uint8_t* buf, size_t size, callback_t on_finish);
	inline void Reset();
	void Decode(uint8_t const* chunk, size_t chunksize);	// возвращает количество отбработанных символов

private:
	inline void decode_symbol(uint8_t byte);
	callback_t OnFinish;
	uint8_t* const buf;
	size_t const buf_size;
	uint8_t* decoded_ptr;
	size_t decoded_len = 0;
	uint8_t lrc = LRCSTART;
	bool is_low = true;
	uint8_t current;
};

inline Decoder::Decoder (uint8_t* _buf, size_t _size, callback_t on_finish) :
	buf(_buf), buf_size(_size), lrc(LRCSTART), decoded_ptr(_buf), OnFinish(on_finish) {
	Reset();
}


inline void Decoder::decode_symbol(uint8_t symbol) {

	if (symbol == FSTART) {
		Reset();
		return;
	}

	if ( symbol == FEND ) {
		if ( lrc == 0 ) {	// Приняли последний символ и сошлась контрольная сумма
			if (OnFinish != nullptr) {
				OnFinish(buf, decoded_len);
				return;
			}
		} else {
		//Reset();
		}
	}

	uint8_t decoded = CodeToByte(symbol);
	if (decoded == 0xff) {	// Приняли неизвестный символ
		Reset();
		return;
	}

	if (is_low) {
		current = decoded;
		is_low = false;
	}
	else {
		current = current | (decoded << 4);
		is_low = true;
		lrc -= current;
		if (decoded_ptr < buf + buf_size) {
			*decoded_ptr = current;
			decoded_ptr++;
			decoded_len++;
		}
	}
}

inline void Decoder::Decode(uint8_t const* chunk, size_t chunksize) {

	while (chunksize > 0) {
		decode_symbol(*chunk++);
		chunksize--;
	}
}

inline void Decoder::Reset() {
	decoded_ptr = buf;
	lrc = LRCSTART;
	//len = 0;
	//is_low = true;
}




} // namespace