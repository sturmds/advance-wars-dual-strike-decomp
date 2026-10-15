#include "types.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
#define DIVISOR(scale) ((scale) ? (scale) : 2)

void func_02049ddc(int x, int y, int digit, int matrixIndex, int frame) {
  int scale;
  int list;

  list = 6;
  if (frame < 0 || frame > 8) {
    Sprite_QueueAt(7, x & 0x1ff, y & 0xff, &data_027c0028, 0x4000,
                   0xe8 + digit * 2);
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
               ((y - 4) & 0xff) | 0x300, &data_027c0028, 0x4000,
               0xe8 + digit * 2);
}
