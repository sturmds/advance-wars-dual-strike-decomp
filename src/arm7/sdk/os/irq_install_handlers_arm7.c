#include "sdk/os_arm7.h"

extern OSIrqFunction data_arm7_03807750
    []; /* handlers for the interrupts that have no slot, indexed by IE bit */
extern IrqSlotEntry
    data_arm7_038078ec; /* data_arm7_0380788c[OS7_IRQ_SLOT_VBLANK] */

/* ARM7 copy of OS_SetIrqFunction. Unlike ARM9, VBlank (bit 0) has a slot too. */
void Irq_InstallHandlers_Arm7(OSIrqMask mask, OSIrqFunction function) {
  int bit;

  for (bit = 0; bit < 25; mask >>= 1, bit++) {
    IrqSlotEntry* slot;

    if ((mask & 1) == 0) {
      continue;
    }
    slot = NULL;
    if (bit >= 8 && bit <= 11) {
      slot = &data_arm7_0380788c[bit - 8];
    } else if (bit >= 3 && bit <= 6) {
      slot = &data_arm7_0380788c[bit + 1];
    } else if (bit == 0) {
      slot = &data_arm7_038078ec;
    } else {
      data_arm7_03807750[bit] = function;
    }
    if (slot != NULL) {
      /* slot handlers take an argument, which is NULL here */
      slot->handler = (void (*)(void*))function;
      slot->arg = NULL;
      slot->keepEnabled = TRUE;
    }
  }
}
