#include "sdk/wl.h"
#include "sdk/mi.h"

// Sets up the RX ring in WIFI_PACKET_RAM: it starts after the TX frames, which take
// more room in some modes (state.unk_24), and runs to WIFI_RX_RING_LIMIT. Clears work
// offsets 0x4b4-0x503 first.
void WifiRx_SetUpRing(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlMpChild* child = WIFI_CHILD_POLL_STATE_OF(work);
  volatile u32
      zero; /* needed to match: the fill value goes through the stack */
  u32 start;
  u16 cursor;

  zero = 0;
  Mem_FillWords_Arm7(zero, child, 0x50);
  REG_W_RXCNT = 0x8000;
  switch (WIFI_DRIVER_STATE->state.unk_24) {
    case 0:
      start = 0x794;
      break;
    case 1:
      start = 0x10c4;
      break;
    case 2:
      start = 0xbfc;
      break;
    case 3:
      start = 0x794;
      break;
  } /* other modes leave start uninitialised, as in the original */
  REG_W_RXBUF_BEGIN = (u32)WIFI_PACKET_RAM + start;
  cursor = start >> 1;
  REG_W_RXBUF_WR_ADDR = cursor;
  REG_W_RXBUF_END = WIFI_RX_RING_LIMIT & 0xffff;
  REG_W_RXBUF_READCSR = cursor;
  child->rxReadCursor = cursor;
  child->lastSequence = 0xffff;
  link->ringSize = WIFI_RX_RING_LIMIT - (u32)WIFI_PACKET_RAM - start;
  REG_W_RXBUF_GAP = (WIFI_RX_RING_LIMIT - 2) & 0xffff;
  REG_W_RXCNT = 0x8001; /* GBATEK: bit 0 loads the write cursor from WR_ADDR */
}
