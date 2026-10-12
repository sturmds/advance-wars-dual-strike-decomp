#include "sdk/wl.h"

// TX start interrupt. With option bit 5 set, when the RF state is 3-5 and
// the hardware sends from between the first MP reply frame and the PS-Poll
// frame in WIFI_PACKET_RAM, pulses bit 7 of register 0x244.
void WifiIrq_TxBegun(void) {
  WlTxSlots* slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);
  u16 rfState;
  u16 txAddr;

  WIFI_IRQ_PENDING_FLAGS = W_IRQ_TX_START;
  if (!(WIFI_DRIVER_STATE->unk_638 & 0x20)) {
    return;
  }
  rfState = REG_W_RF_STATUS & 0xff;
  txAddr = REG_W_RXTX_ADDR;
  if (rfState < 3 || rfState > 5) {
    return;
  }
  if (txAddr <
      (((u32)slots->slot[WIFI_TXLOC_POLL_ANSWER_A].frame >> 1) & 0xfff)) {
    return;
  }
  if (txAddr >
      (((u32)slots->slot[WIFI_TXLOC_GROUP_OR_PSPOLL].frame >> 1) & 0xfff)) {
    return;
  }
  WIFI_MAC_REGISTER(0x244) |= 0x80;
  WIFI_MAC_REGISTER(0x244) &= ~0x80;
}
