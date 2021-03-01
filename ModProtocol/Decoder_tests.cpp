#include "unity/unity_fixture.h"
#include "ZOQ/ModProtocol/decoder.hpp"

using namespace ZOQ::ModProtocol;

uint32_t on_finish_called_times = 0;


TEST_GROUP(ModProtocol_Decoder);
TEST_SETUP(ModProtocol_Decoder) {
	on_finish_called_times = 0;
}
TEST_TEAR_DOWN(ModProtocol_Decoder) { }


const unsigned int buf_size = 100;
unsigned char tmp[buf_size];


void on_finish(uint8_t const* buf, size_t len) {
	on_finish_called_times++;
}

/*
 * Неправильная посылка. Колбэк не будет вызван
 */
TEST(ModProtocol_Decoder, test_bad_message) {
	const uint8_t* msg = ":AA\n";
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 4);
	TEST_ASSERT_EQUAL(0, on_finish_called_times);
}

/*
 * Правильная посылка. Должен быть вызван колбэк
 */
TEST(ModProtocol_Decoder, test_good_message) {
	const uint8_t* msg = ":AA55\n";
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 6);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
}

TEST_GROUP_RUNNER(ModProtocol_Decoder) {
	RUN_TEST_CASE(ModProtocol_Decoder, test_bad_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_good_message);
}