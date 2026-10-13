#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

/* Lock IDs 0x80 to 0xbf belong to the ARM7. */
#define GBA_SLOT_ARM7_OWNER_BIT 0x80

void Cartridge_SpinUntilSlotOwned_Arm7(u16 lockID, GbaSlotLockRecord* lock) {
  for (;;) {
    lock->savedIrqState = Cpu_MaskIrq_Arm7();
    lock->heldBefore =
        LockWord_OwnerId_Arm7(GBA_SLOT_OWNER_WORD) & GBA_SLOT_ARM7_OWNER_BIT;
    if (lock->heldBefore != 0) {
      return;
    }
    if (SlotLock_TryOnce_Arm7(lockID) == 0) {
      return;
    }
    Cpu_RestoreIrqBit_Arm7(lock->savedIrqState);
    Cpu_DelayLoop_Arm7(1);
  }
}
