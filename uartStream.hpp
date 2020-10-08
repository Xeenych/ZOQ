#pragma once
#include <string.h>
#include "hal_uart.hpp"
#include "ITextStream.hpp"

namespace ZOQ::Stm32_HAL {

template<typename T>
class uartStream : public ITextStream<uartStream<T>> {
	public:
		uartStream(T& uart);
		size_t write(uint8_t* buffer, size_t size);
		size_t read(uint8_t* buffer, size_t size);
		size_t writeString(const char* string);		// возвращает длину строки
		size_t readString(char* buf, size_t size);
		void setTimeOut(uint32_t new_timeout);
		uint32_t getTimeOut() const;
		void flush();
	private:
		uint32_t timeout;
		T& uart;
	};

	template<typename T>
	inline void uartStream<T>::setTimeOut(uint32_t new_timeout) {
		timeout = new_timeout;
	}

	template<typename T>
	inline uint32_t uartStream<T>::getTimeOut() const {
		return (timeout);
	}

	template<typename T>
	inline uartStream<T>::uartStream(T& _uart) : timeout(HAL_MAX_DELAY), uart(_uart) {	}

	template<typename T>
	inline size_t uartStream<T>::write(uint8_t* buffer, size_t size) {
		auto transferred = uart.transmit(buffer, size, timeout);
		return transferred;
	}

	template<typename T>
	inline size_t uartStream<T>::read(uint8_t* buffer, size_t size) {
		auto received = uart.receive(buffer, size, timeout);
		return received;
	}

	template<typename T>
	inline size_t uartStream<T>::writeString(const char* string) {
		size_t size = strlen(string);
		auto ret = write((uint8_t*)string, size);
		return ret;
	}

	template<typename T>
	inline size_t uartStream<T>::readString(char* buf, size_t size) {
		char* ptr = buf;
		int num_ch = 0;
		bool lineend_found = false;
		uint8_t symbol;

		while(ptr < buf + size) {
			int result = read (&symbol, 1);
			if (result == 0)
				break;
			*ptr = symbol;
			ptr++;
			num_ch++;
			if ( symbol =='\r') {
				lineend_found = true;
				break;
			}
		}

		*ptr = '\0';

		if (!lineend_found)
			return 0;
		return num_ch;
	}

	template<typename T>
	inline void uartStream<T>::flush() {
		uart.flush();
	}

template<typename T>
static uartStream<T> make_uartStream(T& uart) {
	return (uartStream<T>(uart));
}

} // namespace