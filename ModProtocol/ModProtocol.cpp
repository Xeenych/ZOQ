#include <stdint.h>
#include <stdlib.h>
#include "ModProtocol.hpp"

namespace ZOQ::ModProtocol {

static uint8_t CodeToByte(uint8_t code) {
	uint8_t ret = 0xff;
	switch (code) {
		case '0':
			ret = 0;
			break;
		case '1':
			ret = 1;
			break;
		case '2':
			ret = 2;
			break;
		case '3':
			ret = 3;
			break;
		case '4':
			ret = 4;
			break;
		case '5':
			ret = 5;
			break;
		case '6':
			ret = 6;
			break;
		case '7':
			ret = 7;
			break;
		case '8':
			ret = 8;
			break;
		case '9':
			ret = 9;
			break;
		case 'A':
			ret = 0x0A;
			break;
		case 'B':
			ret = 0x0B;
			break;
		case 'C':
			ret = 0x0C;
			break;
		case 'D':
			ret = 0x0D;
			break;
		case 'E':
			ret = 0x0E;
			break;
		case 'F':
			ret = 0x0F;
			break;
		default:
			return ret;
	}
	return ret;
}


size_t ModProtocol::Encode(const uint8_t* src, size_t srclen,
						   uint8_t* dst, size_t dstlen) {

	size_t reslen = EncodedLen(srclen);
  	if ((reslen > dstlen) || (reslen==0)) 
    	return 0;
  	uint8_t lrc = LRCSTART;
	*dst++ = FSTART;

  	uint8_t hi, lo;
  	for (size_t i=0; i<srclen; i++) {
    	uint8_t b = src[i];
    	hi = (b>>4);
    	lo = b&0x0f;
    	*dst++ = alphabet[hi];
    	*dst++ = alphabet[lo];
    	lrc-=b;
  	}
  	hi = (lrc>>4);
  	lo = lrc&0x0f;
  	*dst++ = alphabet[hi];
  	*dst++ = alphabet[lo];
  	*dst = FEND;
  	return reslen;
}

size_t ModProtocol::Decode(const uint8_t* src, size_t srclen, uint8_t* dst, size_t dstlen) {
	if (srclen&0x01 == 1)
		return 0;

	size_t resultlen = DecodedLen(srclen);
	if (resultlen > dstlen || resultlen==0)
		return 0;

	uint8_t lrc = LRCSTART;
	uint8_t i;
	src++;
    uint8_t hi,lo, dechi, declo, res;
	for (i=0; i<resultlen; i++) {
		hi = src[2*i];
		lo = src[2*i+1];
		dechi = CodeToByte(hi);
		declo = CodeToByte(lo);
		res = (dechi<<4)|declo;
		lrc-=res;
		*dst++ = res;
	}

    hi = src[2*i];
    lo = src[2*i+1];
	dechi = CodeToByte(hi);
	declo = CodeToByte(lo);
	res = (dechi<<4)|declo;

	if (res!=lrc)
		return 0;

  	return resultlen; //  LRC
}

size_t ModProtocol::EncodedLen(size_t srclen) {
	if (srclen > MAX_DECODED) 
		return 0;
	size_t reslen = (srclen<<1) + 1 + 2 + 1; // + ':' + LRC(2) + '\n'
	return reslen;
}

size_t ModProtocol::DecodedLen(size_t srclen) {
	if (srclen > MAX_ENCODED)
		return 0;
	if (srclen < 4)
		return 0;
	size_t reslen = ((srclen-4) >>1);
	return reslen;
}
} // namespace ZOQ::ModProtocol