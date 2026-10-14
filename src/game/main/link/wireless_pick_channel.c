#include "types.h"
#include "game/wireless.h"

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
  channel = HwMod32(MainLoop_GetFrameCount(), WIRELESS_CHANNEL_COUNT) + 1;
  for (i = 1; i <= WIRELESS_CHANNEL_COUNT; i++) {
    if (channel == WIRELESS_CHANNEL_COUNT + 1) {
      channel = 1;
    }
    if (allowed & (1 << (channel - 1))) {
      return channel;
    }
    channel++;
  }
  return 0;
}
