#include "types.h"
#include "sdk/os.h"

#define SLOT2_WORD_0 (*(vu32 *)0x08000000)
#define SLOT2_WORD_1 (*(vu32 *)0x08000004)

#define SLOT2_MAGIC_0 0x544e494e
#define SLOT2_MAGIC_1 0x4f444e45
#define SLOT2_MATCH 0x01000000
#define SLOT2_NO_MATCH 0x02000000

int OS_GetLockID(void);
u16 func_0210cf88(void *lock);
int OS_LockCartridge(u16 lockId);
void OS_UnlockCartridge(u16 lockId);

/*
 * Takes the lock on the shared word, then checks whether Slot-2 memory starts
 * with the text "NINTENDO". The order of the last three locals is needed to
 * match.
 */
u32 func_0210db18(void)
{
    u32 result;
    BOOL done;
    u16 lockId;
    int lockResult;
    int irqState;

    done = FALSE;
    lockId = OS_GetLockID();
    result = 0;

    do {
        lockResult = -1;
        irqState = OS_DisableInterrupts();

        if ((func_0210cf88(SLOTLOCK_WORD) & 0x40) != 0 || (lockResult = OS_LockCartridge(lockId)) == 0) {
            if (SLOT2_WORD_0 == SLOT2_MAGIC_0 && SLOT2_WORD_1 == SLOT2_MAGIC_1) {
                result = SLOT2_MATCH;
            } else {
                result = SLOT2_NO_MATCH;
            }
            if (lockResult == 0) {
                OS_UnlockCartridge(lockId);
                done = TRUE;
            }
        }
        OS_RestoreInterrupts(irqState);
    } while (!done);
    return result;
}
