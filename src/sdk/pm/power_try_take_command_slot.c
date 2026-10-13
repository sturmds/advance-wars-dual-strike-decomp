#include "sdk/os.h"
#include "sdk/pm.h"

/* Takes the command lock. Returns 0 when another command holds it. */
BOOL Power_TryTakeCommandSlot(void) {
  int irq = OS_DisableInterrupts();

  if (gPMWork.lock != 0) {
    OS_RestoreInterrupts(irq);
    return FALSE;
  }
  gPMWork.lock = 1;
  OS_RestoreInterrupts(irq);
  return TRUE;
}
