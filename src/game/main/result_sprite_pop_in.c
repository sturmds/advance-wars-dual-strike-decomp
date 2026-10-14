#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/result_sprite_task.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(ResultSpriteTask* task);
void ResultPath_DrawAll(int step);

extern u8 data_027c0060[];
extern s16 gSineTable[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* One frame of a 4-frame pop-in: draws the four sprites of ResultPath_DrawAll,
 * and the data_027c0060 sprite through affine matrix 1, scaled by an ease
 * from 2x down to 1x, in a double-size box at (184, 80). ResultSprite_Shake
 * then draws it unscaled at (200, 88). Resumes the task's script on the last frame. */
void ResultSprite_PopIn(ResultSpriteTask* task) {
  int scale;

  task->frame++;
  ResultPath_DrawAll(195 - (5 - data_022a7a9c.rank) * 35);
  scale = Math_Ease(0, 0x200, 0x100, task->frame, 4);
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    /* needed to match: declared in this order for the registers */
    int sinValue;
    int cosValue;

    cosValue = gSineTable[64] * 16;
    sinValue = gSineTable[0];

    Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                        HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
  }
  Sprite_Queue(6, 184 | (1 << 9), 80 | 0x300, &data_027c0060, 0x1000, 0x40a);
  if (task->frame == 4) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
