#include "types.h"
#include "game/Unk_02080440.h"

s32 HwDiv32(s32 num, s32 den);
u32 Team_GetSecondsUsed(int unused);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Frames left of the map time limit (timeLimitFrames) after the seconds
 * Team_GetSecondsUsed counts; 0 when it is used up. The argument 1 is passed
 * by the original; Team_GetSecondsUsed ignores it. */
u32 TimeLimit_GetFramesLeft(void) {
  u32 budget =
      HwDiv32(data_027c027c->unk_48[data_027c0284]->timeLimitFrames, 60);
  u32 used = Team_GetSecondsUsed(1);

  if (budget < used) {
    return 0;
  }
  return (budget - used) * 60;
}
