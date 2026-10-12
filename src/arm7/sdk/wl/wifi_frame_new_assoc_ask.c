#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame* frame, const u16* dest);
u32 WifiFrame_PutSsidElement(u8* dest);
u32 WifiFrame_PutRatesElement(u8* dest);

// Allocates a buffer and builds an association request to dest: capability,
// listen interval, then the SSID and rates elements. Returns NULL when no
// buffer is free.
WlTxFrame* WifiFrame_NewAssocAsk(const u16* dest) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  WlTxBuffer* buffer =
      (WlTxBuffer*)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 94);
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
  WIFI_MANAGEMENT_FIELDS_OF(frame)->assocRequest.capability = link->flags_7c;
  WIFI_MANAGEMENT_FIELDS_OF(frame)->assocRequest.listenInterval =
      link->listenInterval;
  length = WifiFrame_PutSsidElement(
      WIFI_MANAGEMENT_FIELDS_OF(frame)->assocRequest.elements);
  length += WifiFrame_PutRatesElement(
      WIFI_MANAGEMENT_FIELDS_OF(frame)->assocRequest.elements + length);
  /* 4: the capability and listen interval before the elements */
  frame->bodyLength = length + 4;
  frame->length = frame->bodyLength + WIFI_HEADER_PLUS_CHECKSUM;
  frame->frameControl = WIFI_FRAMECTL_ASSOC_ASK;
  return frame;
}
