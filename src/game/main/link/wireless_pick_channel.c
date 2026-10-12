#include "types.h"

u16 WM_GetAllowedChannel(void);
int MainLoop_GetFrameCount(void);
s32 HwMod32(s32 numerator, s32 denominator);

/* Picks an allowed channel, starting the search at one chosen by the frame
 * count. Returns 0 when no channel is allowed. */
u16 Wireless_PickChannel(void) {
  u16 allowed;
  int i;
  int channel;

  allowed = WM_GetAllowedChannel();
  if (allowed == 0) {
    return 0;
  }
  channel = HwMod32(MainLoop_GetFrameCount(), 14) + 1;
  for (i = 1; i <= 14; i++) {
    if (channel == 15) {
      channel = 1;
    }
    if (allowed & (1 << (channel - 1))) {
      return channel;
    }
    channel++;
  }
  return 0;
}
