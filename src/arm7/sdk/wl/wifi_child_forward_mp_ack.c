#include "sdk/wl.h"

/*
 * Handles a received MP ack (frame control 0x218: data + CF-Ack from the
 * DS). While associated (mode 0x40) and the frame comes from our BSSID and
 * AP, the buffer goes to the host as indication 0x185 with the reply-enable
 * bits of both MP reply registers added to its RX header word. Returns 0
 * when the buffer went to the host, 1 when the caller keeps it.
 */
int WifiChild_ForwardMpAck(WlRxFrame* frame) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  WlRxBuffer* buffer;

  if (WIFI_DRIVER_STATE->state.mode != WIFI_MODE_JOINED) {
    return 1;
  }
  if (!WifiMac_Equal(frame->addr2, link->bssid) ||
      !WifiMac_Equal(frame->addr3, link->currentAp)) {
    return 1;
  }
  frame->bodyLength = frame->length - WIFI_HEADER_PLUS_CHECKSUM;
  buffer = WIFI_RX_HOLDER_OF(frame);
  buffer->command = 0x185;
  buffer->length = 24;
  buffer->frame.unk_08 |=
      WIFI_CHILD_POLL_STATE_OF(WIFI_DRIVER_STATE)->replyFlags |
      ((WIFI_MAC_REGISTER(0x098) & 0x8000) >> 4) |
      ((WIFI_TX_NEXT_POLL_ANSWER & 0x8000) >> 3);
  WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &buffer->header);
  return 0;
}
