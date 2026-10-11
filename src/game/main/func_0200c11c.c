#include "types.h"

void OS_SetIrqFunction(int a, int b);
void OS_EnableIrqMask(int a);
void CallbackList_Add(void (*callback)(void));
void func_0200c0ec(void);

void func_0200c11c(int arg)
{
    OS_SetIrqFunction(2, arg);
    OS_EnableIrqMask(2);
    CallbackList_Add(func_0200c0ec);
}
