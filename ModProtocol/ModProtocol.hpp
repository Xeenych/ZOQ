#pragma once

namespace ZOQ::ModProtocol {

	static const uint8_t alphabet[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	static const uint8_t FEND = '\n';
	static const uint8_t FSTART = ':';
	static const uint8_t LRCSTART = (-1);
	
	
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