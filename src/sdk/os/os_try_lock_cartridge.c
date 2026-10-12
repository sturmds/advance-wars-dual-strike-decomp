#include "sdk/os.h"

int OSi_AllocateCartridgeBus(void);

/* Returns 0 once the lock is taken. */
int OS_TryLockCartridge(u16 lockId)
{
    return OSi_DoTryLockByWord(lockId, SLOTLOCK_WORD, (void (*)(void))OSi_AllocateCartridgeBus, TRUE);
}
