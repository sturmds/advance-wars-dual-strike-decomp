#include "types.h"
#include "sdk/pm.h"

void func_0210ed9c(u8* mac);

/* A number from 0 to 19 made from the console's MAC address and the frame counter,
 * so that nearby parents do not use the same value. */
u32 MbParent_GetBeaconJitter(void) {
  u8 mac[6];
  u32 sum;
  int i;

  func_0210ed9c(mac);
  sum = i = 0;
  for (; i < 6; i++) {
    sum += mac[i];
  }
  sum += *SHARED_FRAME_COUNTER;
  return sum * 7 % 20;
}
