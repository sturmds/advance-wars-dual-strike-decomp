#include "types.h"

void OS_LockByWord(int, int, void *);
void func_0210cfa8(void);

void func_0210cff8(int a0)
{
    OS_LockByWord(a0, 0x27fffe0, (void *)func_0210cfa8);
}
