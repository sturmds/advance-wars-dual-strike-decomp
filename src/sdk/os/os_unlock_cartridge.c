#include "types.h"

void OSi_DoUnlockByWord(int, int, void *, int);
int OSi_FreeCartridgeBus(void);

void OS_UnlockCartridge(int a0)
{
    OSi_DoUnlockByWord(a0, 0x27fffe8, (void *)OSi_FreeCartridgeBus, 1);
}
