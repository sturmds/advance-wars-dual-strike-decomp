#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

void func_0210cee4(int lockID); /* tail call to OS_UnlockCartridge */

void Cartridge_ReleaseSlotIfTaken(int lockID, GbaSlotLockRecord* lock) {
  if (lock->heldBefore == 0) {
    func_0210cee4(lockID);
  }
  OS_RestoreInterrupts(lock->savedIrqState);
}
