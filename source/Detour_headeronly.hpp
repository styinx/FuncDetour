#ifndef DETOUR_HPP
#define DETOUR_HPP

#include <cstdint>

using Byte = unsigned char;

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
    static void detour(Byte* source_func, const Byte* target_func, const Int func_len)
    {
        // Remote Protection
        DWORD virtual_protect;
        VirtualProtect(source_func, func_len, PAGE_EXECUTE_READWRITE, &virtual_protect);

        // Jump from the source function to the detour
        Int target_address            = (Int)((target_func - (Int)source_func) - 5);
        *source_func                  = '\xE9';          // Jump
        *(Int*)((Int)source_func + 1) = target_address;  // Target address

        // Reintroduce protection
        VirtualProtect(source_func, func_len, virtual_protect, &virtual_protect);
    }

    /**
     * @brief
     * @param source_func
     * @param target_func
     * @param func_len
     * @return
     */
    static Byte* trampoline(Byte* source_func, const Byte* target_func, const Int func_len)
    {
        // Function needs to be at least 5 bytes for (JMP + 4 byte address)
        if(func_len < 5)
            return nullptr;

        void* gate = VirtualAlloc(nullptr, func_len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        memcpy(gate, source_func, func_len);

        // Jump from the gateway to the content of the source function
        Int gateway_address               = ((Int)source_func - (Int)gate) - 5;
        *(Byte*)((Int)gate + func_len)    = 0xE9;
        *(Int*)((Int)gate + func_len + 1) = gateway_address;

        Detour::detour(source_func, target_func, func_len);

        return (Byte*)gate;
    }
};

#endif  // DETOUR_HPP
