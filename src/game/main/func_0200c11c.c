#include "types.h"

void func_0210ce40(int a, int b);
void func_0210cd3c(int a);
void CallbackList_Add(void (*callback)(void));
void func_0200c0ec(void);

void func_0200c11c(int arg)
{
    func_0210ce40(2, arg);
    func_0210cd3c(2);
    CallbackList_Add(func_0200c0ec);
}
