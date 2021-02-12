#pragma once
#include "ModProtocol/ModProtocol.hpp"

namespace ZOQ::ModProtocol {

	class Encoder {
	public:
		Encoder(uint8_t* buf, size_t size);
		bool Start();
		bool Encode(uint8_t const* chunk, size_t chunksize);
		bool Finish();
		size_t Len() const;
	private:
		uint8_t* const buf;
		size_t const size;
		uint8_t* ptr;
		uint8_t lrc = LRCSTART;
	};
	
	inline Encoder::Encoder (uint8_t* _buf, size_t _size) : 
		buf(_buf), size(_size), lrc(LRCSTART), ptr(buf) {
	}
		
	inline bool Encoder::Start() {
		size_t free = (ptr - buf + size);
		if (free < 1)
			return false;
		*ptr++ = FSTART;
		return true;
	}
	
	inline bool Encoder::Finish() {
		size_t free = (ptr - buf + size);
		if (free < 1 + 2)
			return false;
		
		uint8_t hi = (lrc>>4);
  		uint8_t lo = lrc&0x0f;
  		*ptr++ = alphabet[hi];
  		*ptr++ = alphabet[lo];
  		*ptr++ = FEND;
		return true;
	}
	
	inline bool Encoder::Encode(uint8_t const* chunk, size_t chunksize) {
		size_t free = (ptr - buf + size);
		if (free < (chunksize << 1))
			return false;
		
		uint8_t hi, lo;
  		for (size_t i=0; i < chunksize; i++) {
	    	uint8_t b = chunk[i];
    		hi = (b>>4);
    		lo = b&0x0f;
    		*ptr++ = alphabet[hi];
    		*ptr++ = alphabet[lo];
    		lrc-=b;
  		}
		return true;
	}
	
	inline size_t Encoder::Len() const {
		return ptr-buf;
	}
} // namespace ZOQ::ModProtocol
