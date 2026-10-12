#include "sdk/wl.h"

void WifiBeacon_UnmarkTrafficFor(u16 station);

/* Counts a frame for a station as sent; in mode 1 the last one clears the station's TIM bit. */
void WifiStation_UncountQueuedFrame(WlTxFrame* frame) {
  WlStation* station;
  u32 irqMask;
  u16 index;

  index = frame->station;
  station = WifiStation_Entry(index);
  irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

  if (WIFI_DRIVER_STATE->state.unk_24 == 1 && station->queuedFrames == 1) {
    WifiBeacon_UnmarkTrafficFor(index);
  }
  station->queuedFrames--;
  Irq_SetEnableBits_Arm7(irqMask);
}
