#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "common.hpp"

namespace ZOQ::ModProtocol {

	
class Decoder {
	
using callback_t = void (*)(uint8_t const* buf, size_t len);
	
public:
	Decoder(uint8_t* buf, size_t size, callback_t on_finish);
	void Reset();
	size_t Decode(void const* chunk, size_t chunksize);	// возвращает количество отбработанных символов

private:
	callback_t OnFinish;
	uint8_t* const buf;
	size_t const size;
	uint8_t* ptr;
	uint8_t lrc = LRCSTART;	
};

inline Decoder::Decoder (uint8_t* _buf, size_t _size, callback_t on_finish) : 
	buf(_buf), size(_size), lrc(LRCSTART), ptr(buf), OnFinish(on_finish) {
	Reset();
}

inline size_t Decoder::Decode(void const* chunk, size_t chunksize) {
	return 0;
}

inline void Decoder::Reset() {
	ptr = buf;
	lrc = LRCSTART;
}




} // namespace