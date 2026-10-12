#include "types.h"

void OS_UnlockByWord(int, int, void *);
void func_0210cf90(void);

void func_0210cfdc(int a0)
{
    OS_UnlockByWord(a0, 0x27fffe0, (void *)func_0210cf90);
}
