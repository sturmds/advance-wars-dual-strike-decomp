#include "sdk/wl.h"

void WifiBeacon_MarkTrafficFor(u16 station);

/* Counts a frame queued for a station; in mode 1 the first one sets the station's TIM bit. */
void WifiStation_CountQueuedFrame(WlTxFrame *frame)
{
    WlStation *station;
    u32 irqMask;
    u16 index;

    index = frame->station;
    station = WifiStation_Entry(index);
    irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
    if (WIFI_DRIVER_STATE->state.unk_24 == 1 && station->queuedFrames == 0) {
        WifiBeacon_MarkTrafficFor(index);
    }
    station->queuedFrames++;
    Irq_SetEnableBits_Arm7(irqMask);
    station->timeout = station->timeoutReload;
}
