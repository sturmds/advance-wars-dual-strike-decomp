#include "sdk/wl.h"

// Builds the PS-Poll frame in WIFI_PACKET_RAM: the AID with its two top bits set, then
// the BSSID and our MAC address.
void WifiTx_BuildPsPoll(u32 aid) {
  WlRamFrame* frame = WIFI_DRIVER_STATE->state.psPollFrame;

  frame->status = 0;
  frame->unk_02 = 0;
  frame->unk_04 = 0;
  frame->length = 20; /* 16-byte frame and the FCS */
  frame->frameControl = 0xa4;
  frame->duration = aid | 0xc000;
  WifiMac_CopyAddressPair(frame->addr1, WIFI_DRIVER_STATE->state.unk_7c,
                          WIFI_DRIVER_STATE->macAddress);
}
