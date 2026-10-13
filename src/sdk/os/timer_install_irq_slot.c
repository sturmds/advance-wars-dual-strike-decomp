#include "sdk/os.h"

/* The timer entries start at gOsIrqSlots[4]; see IrqSlot_CallAndClear. */
IrqSlotEntry data_022a98ec[0x4]; /* size: up to the next symbol */

/* Sets the handler and its argument for a timer interrupt, then enables it. */
void Timer_InstallIrqSlot(int timer, void (*handler)(void*), void* arg) {
  OSIrqMask mask = 1UL << (timer + 3);

  data_022a98ec[timer].handler = handler;
  data_022a98ec[timer].arg = arg;
  OS_EnableIrqMask(mask);
  data_022a98ec[timer].keepEnabled = TRUE;
}
