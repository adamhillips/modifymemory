#pragma once

#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <Psapi.h>

namespace memory
{
    template <typename T>
    __forceinline bool read(uintptr_t address, T& value)
    {
        if (!address)
        {
            value = T{};
            return false;
        }

        __try
        {
            value = *reinterpret_cast<const T*>(address);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            value = T{};
            return false;
        }
    }

    template <typename T>
    __forceinline T read(uintptr_t address)
    {
        T value{};
        read(address, value);
        return value;
    }

    template <typename T>
    __forceinline bool write(uintptr_t address, const T& value)
    {
        if (!address)
            return false;

        __try
        {
            *reinterpret_cast<T*>(address) = value;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return false;
        }
    }

    template <typename T>
    __forceinline bool read_array(
        uintptr_t address,
        T* buffer,
        size_t count)
    {
        if (!address || !buffer || !count)
            return false;

        __try
        {
            std::memcpy(
                buffer,
                reinterpret_cast<const void*>(address),
                count * sizeof(T));

            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            std::memset(buffer, 0, count * sizeof(T));
            return false;
        }
    }

    template <typename T>
    __forceinline T* ptr(uintptr_t address)
    {
        if (!address)
            return nullptr;

        return reinterpret_cast<T*>(address);
    }

    __forceinline bool is_valid(uintptr_t address)
    {
        if (!address)
            return false;

        __try
        {
            volatile unsigned char test =
                *reinterpret_cast<volatile unsigned char*>(address);

            (void)test;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return false;
        }
    }
}
