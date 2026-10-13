#include "sdk/os.h"

IrqSlotEntry gOsIrqSlots[0x4]; /* size: up to the next symbol */

extern u16 gOsIrqBitBySlot[];
extern u8 data_027c0000[];

/* DTCM flag word that the IRQ wait routines poll, 8 bytes below the end of DTCM. */
#define DTCM_IRQ_SEEN_BITS \
  (*(volatile u32*)((u32)data_027c0000 + 0x3000 + 0xff8))

void IrqSlot_CallAndClear(int slot) {
  OSIrqMask mask;
  void (*handler)(void*);

  mask = 1 << gOsIrqBitBySlot[slot];
  handler = gOsIrqSlots[slot].handler;
  gOsIrqSlots[slot].handler = NULL;
  if (handler != NULL) {
    handler(data_022a98c4[slot].arg);
  }
  DTCM_IRQ_SEEN_BITS |= mask;
  if (data_022a98c0[slot].keepEnabled == 0) {
    OS_DisableIrqMask(mask);
  }
}
