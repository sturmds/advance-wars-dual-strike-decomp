#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/time_dial.h"

int MainLoop_GetFrameCount(void);
s32 HwDiv32(s32 numerator, s32 denominator);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Bg_SetScroll(int layer, int scrollX, int scrollY);
BOOL NumberSpriteTask_IsRunning(void);
BOOL TimeLimit_GetTotal(int* value);
BOOL TimeLimit_GetRemaining(int* value);
void TimeDial_QueueSprite(int index, int x, int y);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern s16 data_0214d4c4[]; /* a sine and cosine pair per 16 angle units */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Per-frame update of a screen with a dial for the time left: scrolls BG
 * layer 2 one pixel every 4 frames, and while NumberSpriteTask_IsRunning finds its task
 * and both the time limit (TimeLimit_GetTotal) and the time left (TimeLimit_GetRemaining)
 * are known, turns the hand from 0 to a quarter turn as the time runs out.
 * The hand keeps its angle while the main screen's unk_1c4->unk_30 is set.
 * Draws sprites 0 and 2 at (136, 128), and sprites 3 and 1 on a circle of
 * radius 128 around (8, 128): straight up at angle 0, at (136, 128) after
 * the quarter turn. The dial is a guess at what the sprites show. */
void TimeDial_Update(TimeDial* dial) {
  int total;
  int left;
  u32 angle;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  u32 keepAngle;
  int idx;
  int handX;
  int handY;

  Bg_SetScroll(2, (MainLoop_GetFrameCount() >> 2) & 0xff, 0);
  if (NumberSpriteTask_IsRunning() && TimeLimit_GetTotal(&total) &&
      TimeLimit_GetRemaining(&left)) {
    angle = 0x4000 - ((u32)HwDiv32(left, 60) << 14) / (u32)HwDiv32(total, 60);
  } else {
    angle = 0;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  keepAngle = STATE->unk_1c4->unk_30;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
  if (keepAngle == 0) {
    dial->angle = angle;
  }
  /* angle 0 points straight up */
  idx = ((dial->angle - 0x4000) & 0xffff) >> 4;
  handX = ((data_0214d4c4[idx * 2 + 1] << 7) >> 12) + 8;
  handY = ((data_0214d4c4[idx * 2] << 7) >> 12) + 128;
  TimeDial_QueueSprite(0, 136, 128);
  TimeDial_QueueSprite(2, 136, 128);
  TimeDial_QueueSprite(3, handX, handY);
  TimeDial_QueueSprite(1, handX, handY);
}
