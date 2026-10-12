#include "types.h"

void OS_TryLockByWord(int, int, void *);
void func_0210cfa8(void);

void func_0210cfc0(int a0)
{
    OS_TryLockByWord(a0, 0x27fffe0, (void *)func_0210cfa8);
}
