#include "unity/unity_fixture.h"
#include "ZOQ/ModProtocol/decoder.hpp"

using namespace ZOQ::ModProtocol;

uint32_t on_finish_called_times = 0;
uint8_t const* result = nullptr;
size_t result_len = 0;

class CB : public Decoder::IDecoderCallback {
public:
	void Execute(uint8_t const* buf, size_t len) override {
		on_finish_called_times++;
		result = buf;
		result_len = len;
	}
};


CB on_finish;


TEST_GROUP(ModProtocol_Decoder);
TEST_SETUP(ModProtocol_Decoder) {
	on_finish_called_times = 0;
	result = nullptr;
	result_len = 0;
}
TEST_TEAR_DOWN(ModProtocol_Decoder) { }


const unsigned int buf_size = 100;
unsigned char tmp[buf_size];



/*
 * Неправильная посылка. Колбэк не будет вызван
 */
TEST(ModProtocol_Decoder, test_bad_message) {
	const uint8_t* msg = (uint8_t*)(":AA\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 4);
	TEST_ASSERT_EQUAL(0, on_finish_called_times);
}

/*
 * Правильная посылка. Должен быть вызван колбэк
 */
TEST(ModProtocol_Decoder, test_good_message) {
	const uint8_t* msg = (uint8_t*)(":AB54\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 6);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
	uint8_t expected[] = {0xab};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result, 1);
	TEST_ASSERT_EQUAL(1, result_len);
}

/*
 * Правильная посылка, но два символа завершщения. Должен быть вызван колбэк один раз
 */
TEST(ModProtocol_Decoder, test_good_message_double_fend) {
	const uint8_t* msg = (uint8_t*)(":AB54\n\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 7);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
	uint8_t expected[] = {0xab};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result, 1);
	TEST_ASSERT_EQUAL(1, result_len);
}

/*
 * Правильная посылка. Должен быть вызван колбэк два раза
 */
TEST(ModProtocol_Decoder, test_good_message_twice) {
	const uint8_t* msg = (uint8_t*)(":AA55\n:AB54\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 12);
	TEST_ASSERT_EQUAL(2, on_finish_called_times);
	uint8_t expected[] = {0xab};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result, 1);
	TEST_ASSERT_EQUAL(1, result_len);
}

TEST(ModProtocol_Decoder, test_bad_good_message) {
	const uint8_t* msg = (uint8_t*)(":AA51\n:AB54\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 12);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
	uint8_t expected[] = {0xab};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result, 1);
	TEST_ASSERT_EQUAL(1, result_len);
}

/*
 * Первая посылка имеет нечетное количество байт
 */
TEST(ModProtocol_Decoder, test_incomplete_message) {
	const uint8_t* msg = (uint8_t*)(":AA5:AB54\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 10);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
	uint8_t expected[] = {0xab};
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, result, 1);
	TEST_ASSERT_EQUAL(1, result_len);
}

/*
 * Проверка крайних случаев
*/
TEST(ModProtocol_Decoder, test_short_message) {
	const uint8_t* msg = (uint8_t*)(":A\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 3);
	TEST_ASSERT_EQUAL(0, on_finish_called_times);
}

TEST(ModProtocol_Decoder, test_short_message2) {
	const uint8_t* msg = (uint8_t*)(":FF\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 4);
	TEST_ASSERT_EQUAL(1, on_finish_called_times);
	TEST_ASSERT_EQUAL(0, result_len);
}

TEST(ModProtocol_Decoder, test_short_message3)	{
	const uint8_t* msg = (uint8_t*)(":\n");
	Decoder dec {tmp, buf_size, on_finish};
	dec.Decode(msg, 2);
	TEST_ASSERT_EQUAL(0, on_finish_called_times);
}



TEST_GROUP_RUNNER(ModProtocol_Decoder) {
	RUN_TEST_CASE(ModProtocol_Decoder, test_bad_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_good_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_good_message_double_fend);
	RUN_TEST_CASE(ModProtocol_Decoder, test_good_message_twice);
	RUN_TEST_CASE(ModProtocol_Decoder, test_bad_good_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_incomplete_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_short_message);
	RUN_TEST_CASE(ModProtocol_Decoder, test_short_message2);
	RUN_TEST_CASE(ModProtocol_Decoder, test_short_message3);
}
