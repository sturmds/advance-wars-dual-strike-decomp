#include "sdk/os.h"

int OSi_FreeCartridgeBus(void);

void OS_UnlockCartridge(u16 lockId)
{
    OSi_DoUnlockByWord(lockId, SLOTLOCK_WORD, (void (*)(void))OSi_FreeCartridgeBus, TRUE);
}
