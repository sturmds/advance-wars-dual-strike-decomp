#include "types.h"
#include "game/Unk_02031a14.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(Unk_02031a14* task);

extern u8 data_027c0030;
extern s16 gSineTable[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* A step of the data_02159d78 script (see func_020318ac). Over frames 0
 * to 16, draws sprite tile 0x2ac at (x - 32, y - 48) through affine matrix
 * task->matrix with double size, turning one full circle while it shrinks from
 * 2x to 1x. Then resumes the script. gSineTable has 256
 * steps per turn (index 64 = cosine of 0). */
void func_02031dc0(Unk_02031a14* task) {
  int scale = Math_Ease(4, 512, 256, task->frame, 16);
  int angle = Math_Ease(0, 0, 256, task->frame, 16) & 0xff;

  /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads; sinValue is declared first but read second. */
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int sinValue;
    int cosValue;

    cosValue = gSineTable[angle + 64] * 16;
    sinValue = gSineTable[angle];

    Oam_SetAffineParams(task->matrix, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  Sprite_Queue(1, ((task->x - 32) & 0x1ff) | (task->matrix << 9),
               ((task->y - 48) & 0xff) | 0x300, &data_027c0030, 0xc000, 0x2ac);
  if (task->frame != 16) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
