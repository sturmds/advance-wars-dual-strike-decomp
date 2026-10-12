#include "sdk/gx.h"
#include "sdk/wmsp.h"

/* Lines are 63.5 us long: 127 half microseconds. */
#define HALF_US_PER_LINE 127

/* Works back from the current line to the time line 0 started and stores it in 64 us units. */
void WMSP_StoreFrameStartTime(void) {
  u32 vcount = *REG_VCOUNT;
  u32 lo;
  u32 hi;
  u32 now;

  /* Read the 32-bit counter in halves; read the high half again if the low half wrapped. */
  lo = W_US_COUNT0;
  hi = W_US_COUNT1;
  now = W_US_COUNT0;
  if (lo > now) {
    hi = W_US_COUNT1;
  }
  now = hi << 16 | now;
  WMSP_FRAME_START_TIME = ((now << 1) - vcount * HALF_US_PER_LINE) >> 7;
}
