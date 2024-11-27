#pragma once

#include "zoq.hpp"
#include <stdlib.h>
#include <string.h>

namespace ZOQ {

	class BufReader : public IRead {
	public:
		inline BufReader(void* data, size_t size);
		inline size_t read(void* buf, size_t len) override;
		inline size_t advance(size_t len);
		inline void* data() const;
		inline size_t len() const;
	private:
		uint8_t* data_ptr;
		size_t size;
	};


	BufReader::BufReader(void* data, size_t sz) :
		data_ptr((uint8_t*)data), size(sz) {
	}

	size_t BufReader::advance(size_t len) {
		if (len > size)
			len = size;
		size -= len;
		data_ptr += len;
		return len;
	}

	void* BufReader::data() const{
		return data_ptr;
	}

	size_t BufReader::len() const {
		return size;
	}

	size_t BufReader::read(void* buf, size_t len)  {
		size_t to_read = (len > size)? size: len;
		memcpy(buf, data_ptr, to_read);
		size -= to_read;
		data_ptr += to_read;
		return to_read;
	}

	template <typename oB, typename iW>
	inline size_t operator>>(oB& b, iW& w) {
		auto data = b.data();
		auto data_len = b.len();
		auto written = w.write(data, data_len);
		b.advance(written);
		return written;
	}
};


