#include "types.h"
#include "sdk/pm.h"

void OS_GetMacAddress(u8* mac);

/* A number from 0 to 19 made from the console's MAC address and the frame counter,
 * so that nearby parents do not use the same value. */
u32 MbParent_GetBeaconJitter(void) {
  u8 mac[MAC_ADDRESS_SIZE];
  u32 sum;
  int i;

  OS_GetMacAddress(mac);
  sum = i = 0;
  for (; i < MAC_ADDRESS_SIZE; i++) {
    sum += mac[i];
  }
  sum += *SHARED_FRAME_COUNTER;
  return sum * 7 % 20;
}
