#pragma once
#include <cstdint>
#include <cstdlib>
#include <span>

namespace peripherals {

class storage_itf {
  public:
    virtual bool save(size_t offset, const std::span<const uint8_t>& data) = 0;
    virtual bool load(size_t offset, const std::span<uint8_t>& data) = 0;

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

    virtual size_t size() = 0;

    constexpr virtual ~storage_itf() = default;
};

}  // namespace peripherals