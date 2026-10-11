#include "types.h"

void OSi_DoLockByWord(int, int, void *, int);
int func_0210d02c(void);

void func_0210d044(int a0)
{
    OSi_DoLockByWord(a0, 0x27fffe8, (void *)func_0210d02c, 1);
}
