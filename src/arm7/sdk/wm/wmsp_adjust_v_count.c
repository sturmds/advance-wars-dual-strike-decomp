#include "sdk/gx.h"
#include "sdk/wmsp.h"

#define VCOUNT_HOLD_LINES_MAX 6

/* Holds the line counter back by up to six lines while WMStatus.unk_d4 is at
   least 127, taking 63 off unk_d4 for each line. */
void WMSP_AdjustVCount(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  int lines; /* needed to match: declared before vcount */
  int vcount = *REG_VCOUNT;
  u32 lag;

  if (vcount >= 208 && vcount < 212) {
    lag = buf->status->unk_d4;
    if (lag >= 127) {
      for (lines = 1; lines < VCOUNT_HOLD_LINES_MAX + 1; lines++) {
        if (lag < lines * 63 + 127) {
          break;
        }
      }
      *REG_VCOUNT = vcount + (1 - lines);
      buf->status->unk_d4 -= lines * 63;
    }
  }
}
