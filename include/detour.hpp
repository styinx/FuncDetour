#include <cstdint>
#include <vector>
#include <windows.h>

// include after windows.h
#include <memoryapi.h>

using Byte = std::uint8_t;
using U8   = std::uint8_t;
using U32  = std::uint32_t;
using U64  = std::uint64_t;

#ifdef _WIN64
using UInt = U64;
#else
using UInt = U32;
#endif

enum Operator
{
    RIP          = 0x25,
    JMP_IMM_NEAR = 0xE9,
    JMP_IMM_FAR  = 0xFF,
};

/*
 * Creates a jump instruction from the src function to the dst function.
 * The jump address is determined by the difference of the pointers (and the offset).
 * Returns the instruction as bytes.
 */
std::vector<Byte> makeJump(void* src, void* dst, UInt offset = 0)
{
    UInt argument = 0;

#ifdef _WIN64
    Byte instruction[14] = {0x00};
    Byte op[]            = {Operator::JMP_IMM_FAR, Operator::RIP, 0x00, 0x00, 0x00, 0x00};

    // The absolute jump address
    argument = (UInt)dst;
#else
    Byte instruction[5] = {0x00};
    Byte op[]           = {Operator::JMP_IMM_NEAR};

    // The relative jump offset
    argument = (UInt)dst - (UInt)src - sizeof(instruction);
#endif

    // Add an optional offset
    argument += offset;

    // Copy operator and argument into the instruction
    memcpy(instruction, op, sizeof(op));
    memcpy(instruction + sizeof(op), &argument, sizeof(argument));

    return std::vector<Byte>(instruction, instruction + sizeof(instruction));
}

/*
 * Jump from the src function into the dst function.
 * Returns the current memory at the beginning of the src function.
 */
std::vector<Byte> installHook(void* src, void* dst)
{
    auto jump = makeJump(src, dst);

    // Remove write protection from the source function for writing the jump instruction.
    DWORD protect;
    VirtualProtect(src, jump.size(), PAGE_EXECUTE_READWRITE, &protect);

    // Save current memory
    std::vector<Byte> original(jump.size(), 0x00);
    memcpy(original.data(), src, jump.size());

    // Write instruction
    memcpy(src, jump.data(), jump.size());

    // Reintroduce write protection
    VirtualProtect(src, jump.size(), protect, &protect);

    return original;
}

/*
 * Restore the original memory of the src function.
 * Requires the buffer returned from 'hook'.
 * Returns the current memory at the beginning of the src function.
 */
std::vector<Byte> uninstallHook(void* src, std::vector<Byte> payload)
{
    // Remove write protection from the source function for writing the jump instruction.
    DWORD protect;
    VirtualProtect(src, payload.size(), PAGE_EXECUTE_READWRITE, &protect);

    // Save current memory
    std::vector<Byte> original(payload.size(), 0x00);
    memcpy(original.data(), src, payload.size());

    // Write original payload
    memcpy(src, payload.data(), payload.size());

    // Reintroduce write protection
    VirtualProtect(src, payload.size(), protect, &protect);

    return original;
}
