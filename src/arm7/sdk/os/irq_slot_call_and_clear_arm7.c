#include "sdk/os_arm7.h"

/* IE bit of each ARM7 IRQ slot */
static const u16 data_arm7_0380773c[] = {
#include "data/arm7/data_arm7_0380773c.inc.c"
};

/*
 * ARM7 copy of IrqSlot_CallAndClear, the shared body of the DMA and timer interrupt
 * handlers: runs the slot's handler once, sets the IRQ check flag and
 * disables the interrupt unless the slot asks to stay enabled.
 */
void IrqSlot_CallAndClear_Arm7(int slot) {
  OSIrqMask mask;
  void (*handler)(void*);

  mask = 1 << data_arm7_0380773c[slot];
  handler = data_arm7_0380788c[slot].handler;
  data_arm7_0380788c[slot].handler = NULL;
  if (handler != NULL) {
    handler(data_arm7_03807894[slot].arg);
  }
  OS7_IRQ_CHECK_FLAGS |= mask;
  if (data_arm7_03807890[slot].keepEnabled == 0) {
    Irq_ClearEnableBits_Arm7(mask);
  }
}
