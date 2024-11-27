#pragma once

#include "periph/mem/mem_itf.hpp"
#include "periph/storage/storage_itf.hpp"
#include "sys/dassert.h"
#include "sys/logging.hpp"

namespace peripherals {

// Класс инкапсулирует область памяти для хранения настроек
class mem_storage_t : public storage_itf {
  public:
    mem_storage_t(mem_itf& mem, size_t offset, size_t sz) : _mem{mem}, _offset{offset}, _sz{sz}
    {
        DASSERT(offset + sz <= mem.size(), "mem is too small");
    }

    bool save(size_t offset, const std::span<const uint8_t>& data) override
    {
        // LOG_D("save offset: %d, size: %d", offset, data.size_bytes());
        return (data.size_bytes() + offset <= _sz) ? _mem.write(_offset + offset, data) : false;
    }

    bool load(size_t offset, const std::span<uint8_t>& data) override
    {
        // LOG_D("load offset: %d, size: %d", offset, data.size_bytes());
        return (data.size_bytes() + offset <= _sz) ? _mem.read(_offset + offset, data) : false;
    }

    template <typename T>
    bool save(size_t offset, const T& value)
    {
        return save(offset, {reinterpret_cast<uint8_t const*>(&value), sizeof(T)});
    }

    template <typename T>
    bool load(size_t offset, T& value)
    {
        return load(offset, {reinterpret_cast<uint8_t*>(&value), sizeof(T)});
    }

    size_t size() override { return _sz; }

    mem_storage_t(const mem_storage_t&) = delete;
    mem_storage_t& operator=(const mem_storage_t&) = delete;

  private:
    mem_itf& _mem;
    const size_t _offset;
    const size_t _sz;
};

}  // namespace peripherals