#include "types.h"
#include "sdk/os_common.h"

void OS_GetMacAddress(u8* mac);

#define VBLANK_COUNT (*(u32*)0x027ffc3c)

/* Scan time per channel in milliseconds, 30 to 39. It depends on the MAC
 * address and the frame counter so that nearby consoles do not scan in step. */
u16 WM_GetDispersionScanPeriod(void) {
  u8 mac[MAC_ADDRESS_SIZE];
  u16 sum;
  int i;

  OS_GetMacAddress(mac);
  sum = i = 0;
  for (; i < MAC_ADDRESS_SIZE; i++) {
    sum += mac[i];
  }
  sum += VBLANK_COUNT;
  sum *= 13;
  return sum % 10 + 30;
}
