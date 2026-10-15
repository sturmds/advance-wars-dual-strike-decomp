#include "types.h"
#include "sdk/os_common.h"
#include "game/math.h"

void SharedInfo_GetMacAddress(u8* mac);

#define VBLANK_COUNT (*(u32*)0x027ffc3c)

/* A number from 0 to 19 made from the console's MAC address and the
 * frame counter at 0x027ffc3c. Wireless_InitParentParam adds it to the
 * beacon period so nearby parents do not all beacon at the same time. */
int Wireless_GetBeaconJitter(void) {
  u8 mac[MAC_ADDRESS_SIZE];
  int sum;
  int i;

  SharedInfo_GetMacAddress(mac);
  sum = i = 0;
  for (; i < MAC_ADDRESS_SIZE; i++) {
    sum += mac[i];
  }
  return HwMod32((sum + VBLANK_COUNT) * 7, 20);
}
