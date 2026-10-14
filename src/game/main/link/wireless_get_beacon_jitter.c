#include "types.h"
#include "sdk/os_common.h"

void OS_GetMacAddress(u8* mac);
s32 HwMod32(s32 numerator, s32 denominator);

#define VBLANK_COUNT (*(u32*)0x027ffc3c)

/* A number from 0 to 19 made from the console's MAC address and the
 * frame counter at 0x027ffc3c. Wireless_InitParentParam adds it to the
 * beacon period so nearby parents do not all beacon at the same time. */
int Wireless_GetBeaconJitter(void) {
  u8 mac[MAC_ADDRESS_SIZE];
  int sum;
  int i;

  OS_GetMacAddress(mac);
  sum = i = 0;
  for (; i < MAC_ADDRESS_SIZE; i++) {
    sum += mac[i];
  }
  return HwMod32((sum + VBLANK_COUNT) * 7, 20);
}
