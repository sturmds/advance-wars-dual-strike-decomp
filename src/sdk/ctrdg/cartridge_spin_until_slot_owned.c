#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

int OS_TryLockCartridge(int lockID);   /* 0 once the lock is taken */
u16 OS_ReadOwnerOfLockWord(void *lockWord);  /* ownerID of an OSLockWord */

void Cartridge_SpinUntilSlotOwned(int lockID, GbaSlotLockRecord *lock)
{
    for (;;) {
        lock->savedIrqState = OS_DisableInterrupts();
        lock->heldBefore = OS_ReadOwnerOfLockWord(GBA_SLOT_OWNER_WORD) & 0x40;
        if (lock->heldBefore != 0) {
            return;
        }
        if (OS_TryLockCartridge(lockID) == 0) {
            return;
        }
        OS_RestoreInterrupts(lock->savedIrqState);
        SVC_WaitByLoop(1);
    }
}
