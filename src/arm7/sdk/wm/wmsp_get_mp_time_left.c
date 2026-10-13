#include "sdk/gx.h"
#include "sdk/math.h"
#include "sdk/wmsp.h"

/* Needed to match: the divide by 20 is inlined, as -O4,p builds it. */
#pragma optimize_for_size off

/* Returns the time left before line 258 in units of 10 us (a frame has 263
   lines of 63.5 us), or 0 when that is shorter than the microseconds an MP
   exchange with these sizes needs. */
u16 WMSP_GetMpTimeLeft(u32 parentSize, u32 childSize, u32 childBitmap) {
  /* Needed to match: one variable for lines and time, declared before needed,
     and the multiply and divide as two statements. */
  int time;
  u32 needed = (parentSize + 34) * 4 + 96 +
               (childSize * MATH_CountPopulation_Arm7(childBitmap) + 904);

  time = 258 - *REG_VCOUNT;
  if (time < 0) {
    time += 263;
  }
  time *= 127;
  time /= 20;
  if (time * 10 < needed) {
    time = 0;
  }
  return time;
}
