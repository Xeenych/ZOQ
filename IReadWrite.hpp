#pragma once


namespace ZOQ {

	class IRead {
	public:
		virtual size_t read (void* buf, size_t size) =0;
	};

	class IWrite {
	public:
		virtual size_t write (void* buf, size_t size) =0;
	};

	class IReadWrite : public IRead, public IWrite {

	};

}