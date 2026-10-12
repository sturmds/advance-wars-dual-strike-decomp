#include "sdk/os.h"

extern BOOL data_arm7_038078f8;   /* set once the locks are set up */
void func_arm7_037fb8e4(void);    /* spins for 0x1000 cycles */

/*
 * ARM7 copy of OS_InitLock. Runs once: waits until the ARM9 has taken the
 * init lock word, marks ARM7 lock IDs 0x80 to 0xaf free (0xb0 to 0xbf are
 * reserved), then tells the ARM9 it is done.
 */
void func_arm7_037fbabc(void)
{
    volatile OSLockWord *initLock;

    if (data_arm7_038078f8 != FALSE) {
        return;
    }
    data_arm7_038078f8 = TRUE;
    initLock = BOOT_HANDSHAKE_WORD;
    initLock->extension = 0;
    while (initLock->ownerID != BOOT_HANDSHAKE_ARM9_READY) {
        func_arm7_037fb8e4();
    }
    OS7_LOCK_ID_FREE_MAP[0] = 0xffffffff;
    OS7_LOCK_ID_FREE_MAP[1] = 0xffff0000;
    initLock->extension = BOOT_HANDSHAKE_ARM7_READY;
}
