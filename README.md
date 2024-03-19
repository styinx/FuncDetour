# FuncDetour

Simple function detour for x86 platforms (32 bit and 64 bit)

## How to use

```c++
#include "detour.hpp"

int add(int a, int b)
{
    return a + b;
}

int hook_add()
{
    return 42;
}

int main(int argc, char** argv)
{
    printf("Original add: %d\n", add(1, 2));  // 3

    auto buffer = installHook(&add, &hook_add);
    printf("Hooked add:   %d\n", add(1, 2));  // 42

    uninstallHook(&add, buffer);
    printf("Original add: %d\n", add(1, 2));  // 3

    return 0;
}
```
