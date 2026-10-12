#include "types.h"

void func_0210ed9c(u8* mac);

#define VBLANK_COUNT (*(u32*)0x027ffc3c)

/* Scan time per channel in milliseconds, 30 to 39. It depends on the MAC
 * address and the frame counter so that nearby consoles do not scan in step. */
u16 WM_GetDispersionScanPeriod(void) {
  u8 mac[6];
  u16 sum;
  int i;

  func_0210ed9c(mac);
  sum = i = 0;
  for (; i < 6; i++) {
    sum += mac[i];
  }
  sum += VBLANK_COUNT;
  sum *= 13;
  return sum % 10 + 30;
}
