#include "sdk/wl.h"

BOOL WifiMp_FreeSentReplySlot(void);

/* Receive done interrupt. Walks the RX ring from rx->readPos to the hardware
   write cursor. Each frame header gets a timestamp (microseconds / 16) and the
   position of the next header; frames too long, or with error bits or a rate
   other than 1 or 2 Mbit/s when unk_638 bit 0 is set, are marked 0xffff and
   counted in WlLink.rxDropped. */
void WifiIrq_RxFinished(void) {
  /* needed to match: this declaration order gives the original's registers */
  u32 options;
  u32 hi2;
  WlLink* link;
  u16* timeField;
  u32 lo2;
  WlRxState* rx;
  u16* header;
  u32 time;
  u32 lo;
  u32 pos;
  u32 end;
  u32 next;
  u32 checkFrames;
  u16* lengthField;
  u32 length;
  u32 offset;
  u32 hi;
  u16* nextField;

  link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  rx = &WIFI_DRIVER_STATE->wepWatch;

  options =
      WIFI_DRIVER_STATE
          ->unk_638; /* needed to match: read before the store, masked after */
  WIFI_IRQ_PENDING_FLAGS = W_IRQ_RX_DONE;
  checkFrames = options & 1;
  for (;;) {
    pos = rx->readPos;
    end = REG_W_RXBUF_WRCSR;

    if (pos == end) {
      break;
    }
    lo = REG_W_US_COUNT0;
    hi = REG_W_US_COUNT1;
    lo2 = REG_W_US_COUNT0;
    hi2 = REG_W_US_COUNT1;
    /* The low half wrapped between the reads: use the second pair. */
    if (lo > lo2) {
      time = (lo2 >> 4) | (hi2 << 12);
    } else {
      time = (lo >> 4) | (hi << 12);
    }
    offset = pos * 2;
    header = (u16*)((u32)WIFI_PACKET_RAM + offset);
    nextField = WifiRx_FoldRingPointer(header + 1);
    timeField = WifiRx_FoldRingPointer(nextField + 1);
    lengthField = WifiRx_FoldRingPointer(timeField + 2);
    /* needed to match: the same address as header, written through offset */
    *(volatile u16*)((u32)WIFI_PACKET_RAM + offset) |=
        (*nextField << 1) & 0x400;
    *timeField = time;
    length = *lengthField;
    /* 12-byte header plus the body, rounded up to a word */
    next = ((offset + length + 15) >> 2) * 2;
    if (next >= (WIFI_RX_RING_LIMIT - (u32)WIFI_PACKET_RAM) / 2) {
      next -= (u32)link->ringSize >> 1;
    }
    if (length > 0x92c) {
      *header = 0xffff;
      next = end;
      link->rxDropped++;
    } else if (checkFrames && next != end) {
      volatile u16* nextHeader = &WIFI_PACKET_RAM[next];
      u16 status = WIFI_PACKET_RAM[next];
      u16 rate;

      if (nextHeader < (volatile u16*)(WIFI_RX_RING_LIMIT - 6)) {
        rate = nextHeader[3] & 0xff;
      } else {
        rate = *(volatile u16*)((u8*)nextHeader - link->ringSize + 6);
      }
      if ((status & 0x7c00) ||
          (rate != WIFI_RATE_CODE_1M && rate != WIFI_RATE_CODE_2M) ||
          length > 0xfff) {
        link->rxDropped++;
        *header = 0xffff;
        rx->readPos = end;
        *nextField = end;
        break;
      }
    }
    if ((*header & 0xf) == 12) {
      if (!WifiMp_FreeSentReplySlot()) {
        WIFI_DRIVER_STATE->counters[27]++;
      }
    }
    rx->readPos = next;
    *nextField = next;
  }
  if (checkFrames) {
    end = REG_W_RXBUF_WRCSR;

    if (WifiMp_FreeSentReplySlot() && end == REG_W_RXBUF_WRCSR) {
      WifiHost_RaiseEventBits(0x20);
    }
  }
  if (REG_W_RXBUF_READCSR != REG_W_RXBUF_WRCSR) {
    WifiTask_Schedule(0, 15);
  }
}
