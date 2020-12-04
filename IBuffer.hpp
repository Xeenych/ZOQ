#pragma once
#include <stdlib.h>
#include <string.h>
#include "zoq.hpp"

namespace ZOQ {

	class oBuffer {
	public:
		inline oBuffer(void* data, size_t size);
		inline size_t read(void* buf, size_t len);
		inline void advance(size_t len);
		inline void* data() const;
		inline size_t data_len() const;
	private:
		size_t size;
		uint8_t* data_ptr;
	};


	oBuffer::oBuffer(void* data, size_t sz) :
		data_ptr((uint8_t*)data), size(sz) {
	}

	void oBuffer::advance(size_t len) {
		if (len > size)
			len = size;
		size -= len;
		data_ptr += len;
	}

	void* oBuffer::data() const{
		return data_ptr;
	}

	size_t oBuffer::data_len() const {
		return size;
	}

	size_t oBuffer::read(void* buf, size_t len) {
		size_t to_read = (len > size)? size: len;
		memcpy(buf, data_ptr, to_read);
		size -= to_read;
		data_ptr += to_read;
		return to_read;
	}

	inline size_t operator>>(oBuffer& b, IWrite& w) {
		auto data = b.data();
		auto data_len = b.data_len();
		auto written = w.write(data, data_len);
		b.advance(written);
		return written;
	}

}