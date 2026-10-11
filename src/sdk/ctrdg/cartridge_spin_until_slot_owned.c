#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

int OS_LockCartridge(int lockID);   /* 0 once the lock is taken */
u16 func_0210cf88(void *lockWord);  /* ownerID of an OSLockWord */

void Cartridge_SpinUntilSlotOwned(int lockID, GbaSlotLockRecord *lock)
{
    for (;;) {
        lock->savedIrqState = OS_DisableInterrupts();
        lock->heldBefore = func_0210cf88(GBA_SLOT_OWNER_WORD) & 0x40;
        if (lock->heldBefore != 0) {
            return;
        }
        if (OS_LockCartridge(lockID) == 0) {
            return;
        }
        OS_RestoreInterrupts(lock->savedIrqState);
        WaitByLoop(1);
    }
}
