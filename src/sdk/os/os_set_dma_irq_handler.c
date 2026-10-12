#include "sdk/os.h"

/* Sets the handler for DMA channel dma (0-3) and enables its interrupt;
 * keepEnabled records whether the interrupt was already enabled. */
void OSi_EnterDmaCallback(u32 dma, void (*handler)(void*), void* arg) {
  u32 mask = 1 << (dma + 8);

  gOsIrqSlots[dma].handler = handler;
  gOsIrqSlots[dma].arg = arg;
  gOsIrqSlots[dma].keepEnabled = OS_EnableIrqMask(mask) & mask;
}
