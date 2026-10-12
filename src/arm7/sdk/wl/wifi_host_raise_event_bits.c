#include "sdk/wl.h"

/* Adds event bits for the host (WlState.unk_c8) and queues task 21 (WifiTask_ReportEvents) to send them. */
void WifiHost_RaiseEventBits(u16 events) {
  OSIrqMask irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

  WIFI_DRIVER_STATE->state.unk_c8 |= events;
  Irq_SetEnableBits_Arm7(irqMask);
  WifiTask_Schedule(2, 21);
}
