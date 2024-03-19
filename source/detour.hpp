#ifndef DETOUR_HPP
#define DETOUR_HPP

#include <cstdint>
#include <windows.h>

using Byte = unsigned char;

#define _WIN32 1

#if INTPTR_MAX == INT64_MAX
using Int = std::uint64_t;
#elif INTPTR_MAX == INT32_MAX
using Int = std::uint32_t;
#endif

class Detour
{
public:
    /**
     * @brief
     * @param source_func
     * @param target_func
     * @param func_len
     */
    static void detour(Byte* source_func, const Byte* target_func, const Int func_len);

    /**
     * @brief
     * @param source_func
     * @param target_func
     * @param func_len
     * @return
     */
    static Byte* trampoline(Byte* source_func, const Byte* target_func, const Int func_len);
};

#endif  // DETOUR_HPP
