#include "sdk/wl.h"

void Wifi_ArmOneShotUs(u32 microseconds, OSAlarmHandler handler);
void WifiMp_ReplyWaitExpired(void* arg);

/* MP command finished. When waitReplies is set (and unk_638 bit 4), the RF
   is in state 3 or 5 and nothing is sending, it sets an alarm for the time
   the polled children need to reply; otherwise it queues task 16. */
void WifiIrq_MpCommandDone(int waitReplies) {
  WlTxSlotView* mp = WIFI_TX_SLOT_RECORDS_OF(WIFI_DRIVER_STATE);

  WIFI_IRQ_PENDING_FLAGS = W_IRQ_MP_END;
  if (mp->txSlots[3].busy == 0) {
    return;
  }
  if ((WIFI_DRIVER_STATE->unk_638 & 0x10) && waitReplies) {
    u16 busy = WIFI_MAC_REGISTER(0x0b6);
    u16 rfState = REG_W_RF_STATUS;

    if ((rfState == 3 || rfState == 5) && busy == 0) {
      u32 count; /* needed to match: declared first */
      WlRamFrame* frame = mp->txSlots[3].frame;
      u32 children = frame->unk_02;

      count = 0;
      while (children != 0) {
        count += children & 1;
        children = (u16)(children >> 1);
      }
      Wifi_ArmOneShotUs(
          (u16)(count * (frame->body[0] + 10) + 192 + frame->length * 4),
          WifiMp_ReplyWaitExpired);
      WIFI_DRIVER_STATE->state.mpReplyWaits++;
      return;
    }
  }
  WifiTask_Schedule(0, 16);
}
