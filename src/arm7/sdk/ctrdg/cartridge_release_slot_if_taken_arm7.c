#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

void Cartridge_ReleaseSlotIfTaken_Arm7(u16 lockID, GbaSlotLockRecord* lock) {
  if (lock->heldBefore == 0) {
    SlotLock_ReleaseAlias_Arm7(lockID);
  }
  Cpu_RestoreIrqBit_Arm7(lock->savedIrqState);
}
