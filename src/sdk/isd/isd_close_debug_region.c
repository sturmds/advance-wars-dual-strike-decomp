#include "types.h"
#include "sdk/isd.h"

/* Undoes Isd_OpenDebugRegion, in the reverse order. */
void Isd_CloseDebugRegion(IsdRegionState* saved) {
  Isd_SetDataPermissions(saved->dataPermissions);
  Isd_SetInstructionPermissions(saved->instructionPermissions);
  Isd_SetProtectionRegion7(saved->region7);
  Isd_RestoreFiq(saved->fiq);
  Isd_RestoreIrq(saved->irq);
}
