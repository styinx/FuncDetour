#include "detour.hpp"

#include <stdio.h>

#pragma optimize("", off)
int add(int a, int b)
{
    return a + b;
}

int hook_add()
{
    return 42;
}
#pragma optimize("", on)

int main()
{
    printf("Original add: %d\n", add(1, 2));

    auto buffer = installHook(&add, &hook_add);
    printf("Hooked add:   %d\n", add(1, 2));

    uninstallHook(&add, buffer);
    printf("Original add: %d\n", add(1, 2));

    return 0;
}
