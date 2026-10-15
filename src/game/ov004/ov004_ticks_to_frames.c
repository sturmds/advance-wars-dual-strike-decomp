#include "types.h"
#include "game/math.h"

/* Ticks of the 64-cycle clock (33514 cycles per millisecond), as 60 Hz frames. */
int Ov004_TicksToFrames(u64 ticks) {
  return HwDiv32((ticks << 6) / 0x82ea * 60, 1000);
}
