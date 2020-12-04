#pragma once
#include <stdlib.h>

namespace ZOQ {

	class IBuffer {
	public:
		virtual size_t read (void* buf, size_t size) =0;
		virtual size_t write (void* buf, size_t size) =0;
	};
}