#include "types.h"

void func_0210ce40(int a, int b);
void OS_EnableIrqMask(int a);
void CallbackList_Add(void (*callback)(void));
void func_0200c0ec(void);

void func_0200c11c(int arg)
{
    func_0210ce40(2, arg);
    OS_EnableIrqMask(2);
    CallbackList_Add(func_0200c0ec);
}
