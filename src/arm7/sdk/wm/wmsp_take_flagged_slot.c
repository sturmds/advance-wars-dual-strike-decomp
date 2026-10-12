#include "types.h"
#include "sdk/wmsp.h"
#include "sdk/os_arm7.h"

WmspSlot* Wmsp_TakeFlaggedSlot(void) {
  WmspSlot* found = NULL;
  int i;
  int irqState = Cpu_MaskIrq_Arm7();
  WmspBuf* buf = data_arm7_027d7c44.buf;

  if (buf != NULL) {
    for (i = 0; i < WMSP_SLOT_COUNT; i++) {
      if (buf->slots[i].flags & WMSP_SLOT_FLAG) {
        found = &buf->slots[i];
        buf->slots[i].flags &= ~WMSP_SLOT_FLAG;
        break;
      }
    }
  }
  Cpu_RestoreIrqBit_Arm7(irqState);
  return found;
}
