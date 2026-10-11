#include "types.h"

void OSi_DoLockByWord(int, int, void *, int);
int OSi_AllocateCartridgeBus(void);

void OS_LockCartridge(int a0)
{
    OSi_DoLockByWord(a0, 0x27fffe8, (void *)OSi_AllocateCartridgeBus, 1);
}
