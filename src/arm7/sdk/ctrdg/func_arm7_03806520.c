#include "types.h"
#include "sdk/ctrdg.h"

int func_arm7_037fdb80(void);              /* OS_DisableInterrupts */
void func_arm7_037fdb94(int state);        /* OS_RestoreInterrupts */
u16 func_arm7_037fb8f4(void *lockWord);    /* ownerID of an OSLockWord */
int func_arm7_037fb904(int lockID);        /* OS_LockCartridge: 0 once the lock is taken */
void func_arm7_037fdc04(u32 count);        /* SVC_WaitByLoop through a veneer */

/* Lock IDs 0x80 to 0xbf belong to the ARM7. */
#define GBA_SLOT_ARM7_OWNER_BIT 0x80

void func_arm7_03806520(int lockID, GbaSlotLockRecord *lock)
{
    for (;;) {
        lock->savedIrqState = func_arm7_037fdb80();
        lock->heldBefore = func_arm7_037fb8f4(GBA_SLOT_OWNER_WORD) & GBA_SLOT_ARM7_OWNER_BIT;
        if (lock->heldBefore != 0) {
            return;
        }
        if (func_arm7_037fb904(lockID) == 0) {
            return;
        }
        func_arm7_037fdb94(lock->savedIrqState);
        func_arm7_037fdc04(1);
    }
}
