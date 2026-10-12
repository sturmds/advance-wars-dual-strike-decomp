#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame* frame, const u16* dest);
u32 WifiFrame_PutSsidElement(u8* dest);
u32 WifiFrame_PutRatesElement(u8* dest);
void func_arm7_027c90ec(void* dest,
                        const void* src); /* copies a 6-byte MAC address */

// Allocates a buffer and builds a reassociation request to dest: capability,
// listen interval, the current AP's address, then the SSID and rates
// elements. Returns NULL when no buffer is free.
WlTxFrame* WifiFrame_NewReassocAsk(const u16* dest) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  WlTxBuffer* buffer =
      (WlTxBuffer*)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 100);
  WlTxFrame* frame;
  u32 length;

  if (buffer == NULL) {
    WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
    return (WlTxFrame*)
        buffer; /* needed to match: NULL, but returned from the register */
  }
  buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
  frame = &buffer->frame;
  WifiTx_FillHeader(frame, dest);
  WIFI_MANAGEMENT_FIELDS_OF(frame)->reassocRequest.capability = link->flags_7c;
  WIFI_MANAGEMENT_FIELDS_OF(frame)->reassocRequest.listenInterval =
      link->listenInterval;
  func_arm7_027c90ec(WIFI_MANAGEMENT_FIELDS_OF(frame)->reassocRequest.currentAp,
                     link->currentAp);
  length = WifiFrame_PutSsidElement(
      WIFI_MANAGEMENT_FIELDS_OF(frame)->reassocRequest.elements);
  length += WifiFrame_PutRatesElement(
      WIFI_MANAGEMENT_FIELDS_OF(frame)->reassocRequest.elements + length);
  /* 10: the capability, listen interval and AP address before the elements */
  frame->bodyLength = length + 10;
  frame->length = frame->bodyLength + WIFI_HEADER_PLUS_CHECKSUM;
  frame->frameControl = WIFI_FRAMECTL_REASSOC_ASK;
  return frame;
}
