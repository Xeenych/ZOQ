#pragma once
#include <stdlib.h>

namespace ZOQ {

template<typename T, size_t SIZE>
class CyclicBufferT {
public:
	CyclicBufferT();
	size_t Capacity() const;
	bool Push(const T &item);
	size_t Push(T const *items, size_t item_count);
	bool Pop(T &item);
	void Remove(size_t n);
	bool IsFull() const;
	bool IsEmpty() const;
	void Clear();
	size_t Count() const; // количество объектов
	size_t ChunkLen() const; // длина непрерывного кучка данных Количество объектов
	size_t ChunkLenBytes() const; // длина непрерывного кучка данных в байтах
	T const* read_pointer() const; // возвращает указатель на начло куска данных
	T* write_pointer() const;// возвращает указатель на начало свободного места
	size_t cont_write() const;// возвращает размер непрерывного куска для записи в объектах
	size_t cont_read() const;// возвращает размер непрерывного куска для чтения в объектах

private:
	void advanceWP();
	void advanceRP();
	size_t wp = 0;
	size_t rp = 0;
	T data[SIZE];
	bool Ovf = false;
};

template<typename T, size_t SIZE>
CyclicBufferT<T, SIZE>::CyclicBufferT() {
}

template<typename T, size_t SIZE>
void CyclicBufferT<T, SIZE>::Clear() {
	wp = rp = 0;
	Ovf = false;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::Capacity() const {
	return SIZE;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::Push(T const *buf, size_t len) {
	size_t i = 0;
	for (i = 0; i < len; i++)
		if (!Push(buf[i]))
			break;
	return i;
}

template<typename T, size_t SIZE>
bool CyclicBufferT<T, SIZE>::IsEmpty() const {
	if ((rp == wp) && !Ovf)
		return true;
	return false;
}

template<typename T, size_t SIZE>
bool CyclicBufferT<T, SIZE>::IsFull() const {
	if ((rp == wp) && Ovf)
		return true;
	return false;
}

template<typename T, size_t SIZE>
bool CyclicBufferT<T, SIZE>::Push(const T &item) {
	if (IsFull())
		return false;
	data[wp] = item;
	advanceWP();
	return true;
}

template<typename T, size_t SIZE>
bool CyclicBufferT<T, SIZE>::Pop(T &item) {
	if (IsEmpty())
		return false;
	item = data[rp];
	advanceRP();
	return true;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::Count() const {
	if (IsFull())
		return SIZE;
	else
		return (wp >= rp) ? wp - rp : wp + SIZE - rp;
}

template<typename T, size_t SIZE>
void CyclicBufferT<T, SIZE>::advanceWP() {
	if (wp < SIZE - 1)
		wp++;
	else
		wp = 0;

	if (wp == rp)
		Ovf = true;
}

template<typename T, size_t SIZE>
void CyclicBufferT<T, SIZE>::advanceRP() {
	if (rp < SIZE - 1)
		rp++;
	else
		rp = 0;

	Ovf = false;
}

template<typename T, size_t SIZE>
void CyclicBufferT<T, SIZE>::Remove(size_t n) {
	size_t len = Count();
	size_t toremove = (len > n) ? n : len;
	for (size_t i = 0; i < toremove; i++)
		advanceRP();
	Ovf = false;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::ChunkLen() const {
	size_t chunk = SIZE - rp;
	size_t len = Count();
	size_t ret = (len > chunk) ? chunk : len;
	return ret;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::ChunkLenBytes() const {
	return ChunkLen() * sizeof(T);
}

template<typename T, size_t SIZE>
T const* CyclicBufferT<T, SIZE>::read_pointer() const {
	return &data[rp];
}

template<typename T, size_t SIZE>
T* CyclicBufferT<T, SIZE>::write_pointer() const {
	return &data[wp];
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::cont_write() const {
	if (wp > rp)
		return SIZE - wp;
	else if (wp < rp)
		return rp - wp;
	else if (Ovf)
		return 0;
	else
		return SIZE;
}

template<typename T, size_t SIZE>
size_t CyclicBufferT<T, SIZE>::cont_read() const {
	if (rp > wp)
		return SIZE - rp;
	else if (rp < wp)
		return wp - rp;
	else if (Ovf)
		return 0;
	else
		return SIZE;
}

} // namesapce ZOQ
