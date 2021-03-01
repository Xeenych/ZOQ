#pragma once
#include "common.hpp"

namespace ZOQ::ModProtocol {
	
	class ModProtocol {
	public:
		static size_t Encode(const uint8_t* src, size_t srclen, uint8_t* dst, size_t dstlen);
		static size_t EncodedLen(size_t srclen);
		static size_t Decode(const uint8_t* src, size_t srclen, uint8_t* dst, size_t dstlen);
		static size_t DecodedLen(size_t srclen);
		static constexpr size_t MAX_ENCODED = 254;
		static constexpr size_t MAX_DECODED = (MAX_ENCODED - 4) / 2;
	};
}	// namespace ZOQ::ModProtocol