#include "sdk/os_arm7.h"

extern BOOL data_arm7_038078f8; /* set once the locks are set up */

/*
 * ARM7 copy of OS_InitLock. Runs once: waits until the ARM9 has taken the
 * init lock word, marks ARM7 lock IDs 0x80 to 0xaf free (0xb0 to 0xbf are
 * reserved), then tells the ARM9 it is done.
 */
void LockId_HandshakeAndFreeIds_Arm7(void) {
  volatile OSLockWord* initLock;

  if (data_arm7_038078f8 != FALSE) {
    return;
  }
  data_arm7_038078f8 = TRUE;
  initLock = BOOT_HANDSHAKE_WORD;
  initLock->extension = 0;
  while (initLock->ownerID != BOOT_HANDSHAKE_ARM9_READY) {
    LockWord_PauseBetweenTries_Arm7();
  }
  OS7_LOCK_ID_FREE_MAP[0] = 0xffffffff;
  OS7_LOCK_ID_FREE_MAP[1] = 0xffff0000;
  initLock->extension = BOOT_HANDSHAKE_ARM7_READY;
}
