#include <stdint.h>

namespace ZOQ::ModProtocol {

	uint8_t CodeToByte(uint8_t code) {
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

}