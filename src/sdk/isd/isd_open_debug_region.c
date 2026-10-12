#include "types.h"
#include "sdk/isd.h"

extern u32 Isd_gDebugRegion7Setting;

/*
 * Masks interrupts and opens protection region 7 for reads and writes, saving
 * what it changes. When Isd_gDebugRegion7Setting holds a region setting, region 7 is set
 * to it first.
 */
void Isd_OpenDebugRegion(IsdRegionState* saved) {
  saved->irq = Isd_DisableIrq();
  saved->fiq = Isd_DisableFiq();
  saved->region7 = Isd_GetProtectionRegion7();
  saved->instructionPermissions = Isd_GetInstructionPermissions();
  saved->dataPermissions = Isd_GetDataPermissions();
  if (Isd_gDebugRegion7Setting != 0) {
    Isd_SetProtectionRegion7(Isd_gDebugRegion7Setting);
  }
  Isd_SetInstructionPermissions(
      (saved->instructionPermissions & ~ISD_REGION7_ACCESS_MASK) |
      ISD_REGION7_ACCESS_READ_WRITE);
  Isd_SetDataPermissions((saved->dataPermissions & ~ISD_REGION7_ACCESS_MASK) |
                         ISD_REGION7_ACCESS_READ_WRITE);
}
