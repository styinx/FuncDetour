# DLLInject

Redirect function calls via a detour.

## Build

```
mkdir build
cd build
cmake ..
```

## How to use

See also `/examples` folder.

```c++
#include "Detour.hpp"

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
    Detour::trampoline((Byte*)add, (Byte*)hook_add, 16);
    add(5, 3); // 42
    return 0;
}
```
