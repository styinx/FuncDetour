#include "Detour.hpp"

#include <iostream>

typedef int (*AddSignature)(int x, int y);
AddSignature original_add;

int add(int a, int b)
{
    return a + b;
}

int hook_add()
{
    return 42;
}

int modify_add(int a, int b)
{
    return original_add(a, b) * 2;
}

int main()
{
    // 'original_add' contains the contents of the original add(...) function.
    // 'add' will jump to the 'hook_add' function.
    original_add = (AddSignature)Detour::trampoline((Byte*)add, (Byte*)hook_add, 16);

    // 8 as expected
    std::cout << original_add(5, 3) << std::endl;

    // 42 from the hook_add
    std::cout << add(5, 3) << std::endl;

    // 16 from the result of 'original_add' * 2
    std::cout << modify_add(5, 3) << std::endl;
}
