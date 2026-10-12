#include "sdk/wl.h"

/* Task 21: sends the pending event bits to the host in indication 0x186 and clears them. */
void WifiTask_ReportEvents(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlIndication* ind;
  OSIrqMask irqMask;

  if (link->pendingEvents != 0) {
    ind = (WlIndication*)WifiBuffer_New(&work->unk_15c, 18);
    if (ind != NULL) {
      ind->command = 0x186;
      ind->length = 1;
      irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
      ind->events = link->pendingEvents;
      link->pendingEvents = 0;
      Irq_SetEnableBits_Arm7(irqMask);
      WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
    }
  }
}
