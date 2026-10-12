#include "types.h"

void Sprite_Queue(int list, int packedX, int packedY, void* data, int arg4,
                  int arg5);

extern u8 gCoPortraitSprite[];

void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list,
                            BOOL flip) {
  u16 xHigh = x & 0xfe00;
  u16 yHigh = y & 0xff00;

  y = (y - 32) & 0xff;
  x &= 0x1ff;
  if (flip) {
    x |= 0x1000;
  }
  Sprite_Queue(list, x | xHigh, y | yHigh, gCoPortraitSprite, arg2, arg3);
}
