#pragma once

namespace ZOQ {

	template <typename T>
	class ITextStream {
	public:
		auto write (  uint8_t* buffer, size_t size);
		auto read (uint8_t* buffer, size_t size);
		auto writeString (const char* string);		// возвращает количество символов или -1 в случе ошибки
		auto readString (char* buf, size_t size); // возвращает количество прочитанных символов
		void setTimeOut (uint32_t new_timeout_ms);
		uint32_t getTimeOut();
	};

	template <typename T>
		auto ITextStream<T>::write(  uint8_t* buffer, size_t size) {
			return static_cast<T*>(this)->write(buffer, size);
		}

	template <typename T>
		auto ITextStream<T>::read(uint8_t* buffer, size_t size) {
			return static_cast<T*>(this)->read(buffer, size);
		}

	template <typename T>
		auto ITextStream<T>::writeString(const char* string) {
			return static_cast<T*>(this)->writeString(string);
		}

	template <typename T>
		auto ITextStream<T>::readString(char* buf, size_t size) {
			return static_cast<T*>(this)->readString(buf, size);
		}

	template <typename T>
		void ITextStream<T>::setTimeOut(uint32_t new_timeout_ms) {
			return static_cast<T*>(this)->setTimeOut(new_timeout_ms);
		}
	template <typename T>
		uint32_t ITextStream<T>::getTimeOut() {
			return static_cast<T*>(this)->getTimeOut();
		}
}