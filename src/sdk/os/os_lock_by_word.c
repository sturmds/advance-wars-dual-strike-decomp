#include "types.h"

void OSi_DoLockByWord(int, int, int, int);

void OS_LockByWord(int a0, int a1, int a2)
{
    OSi_DoLockByWord(a0, a1, a2, 0);
}
