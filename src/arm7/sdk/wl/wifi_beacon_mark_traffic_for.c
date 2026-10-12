#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
void WL_WriteByte(void* addr, u32 value);

/*
 * Marks buffered frames for a station in the beacon's TIM element: station 0
 * sets the multicast bit (bitmap control, byte 4), any other station the bit
 * of its association ID in the partial virtual bitmap from byte 5.
 */
void WifiBeacon_MarkTrafficFor(u16 station) {
  u8* tim;
  u32 irqMask;
  u8* address;
  u32 bits;
  u32 aid;

  if (WifiStation_StateOf(station) != WIFI_PEER_ASSOCIATED) {
    return;
  }
  tim = (u8*)(WIFI_DRIVER_STATE->state.timOffset + WIFI_RAM_TIM_BASE);
  irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
  if (station == 0) {
    bits = WL_ReadByte(tim + 4) | 1;
    /* Needed to match: the original narrows the value at the call. */
    WL_WriteByte(tim + 4, (u8)bits);
  } else {
    aid = WifiStation_AssocIdOf(station);
    address = tim + 5 + (aid >> 3);
    bits = WL_ReadByte(address) | (1 << (aid & 7));
    WL_WriteByte(address, (u8)bits);
  }
  Irq_SetEnableBits_Arm7(irqMask);
}
