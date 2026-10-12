#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
extern u8 data_027c0038;
extern s16 gSineTable[];
#define DIVISOR(scale) ((scale) ? (scale) : 2)

void func_02034654(int x, int y, int digit, int matrixIndex, int frame) {
  int scale;
  int list;

  list = 5;
  if (frame < 0 || frame > 8) {
    Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0038, 0xc000,
                   0x314 + digit * 2);
    return;
  }
  scale = Math_Ease(4, 0x200, 0x100, frame, 8);
  {
    int d3 = DIVISOR(scale);
    int d2 = DIVISOR(scale);
    int d1 = DIVISOR(scale);
    int d0 = DIVISOR(scale);
    int cosValue = gSineTable[64] * 16;
    int sinValue = gSineTable[0];

    Oam_SetAffineParams(matrixIndex, HwDiv32(cosValue, d0),
                        HwDiv32(-sinValue * 16, d1), HwDiv32(sinValue * 16, d2),
                        HwDiv32(cosValue, d3));
  }
  list -= (scale - 0x100) >> 6;
  Sprite_Queue(list, ((x - 4) & 0x1ff) | (matrixIndex << 9),
               ((y - 8) & 0xff) | 0x300, &data_027c0038, 0xc000,
               0x314 + digit * 2);
}
