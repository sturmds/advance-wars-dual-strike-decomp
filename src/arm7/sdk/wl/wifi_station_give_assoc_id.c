#include "sdk/wl.h"

/* Association IDs run from 1 to WIFI_ASSOC_ID_END - 1. */
#define WIFI_ASSOC_ID_END 16

void func_arm7_027c92b8(void);

u32 WifiStation_GiveAssocId(u16 station) {
  WlStationBits* bits = &WIFI_DRIVER_STATE->stationBits;
  u32 aid;
  u32 bit;
  u32 irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

  for (aid = 1, bit = 2; aid < WIFI_ASSOC_ID_END; aid++, bit <<= 1) {
    if (!(bits->aidMask & bit)) {
      bits->aidMask |= bit;
      bits->aidCount++;
      if (bits->aidCount == 1) {
        func_arm7_027c92b8();
      }
      WifiStation_Entry(station)->aid = aid;
      Irq_SetEnableBits_Arm7(irqMask);
      return aid;
    }
  }
  Irq_SetEnableBits_Arm7(irqMask);
  return 0;
}
