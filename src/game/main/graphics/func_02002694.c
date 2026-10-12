#include "types.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_02151d38[];

#define OBJ_X_FLIP 0x1000

/* Queues the sprite group data_02151d38 on sprite list `list`, drawn 32
 * pixels above y. The bits of x above 0x1ff and of y above 0xff pass through
 * as OAM attribute bits; mirror flips the group horizontally. */
void func_02002694(int x, int y, int attr, int list, int mirror) {
  u16 xAttr = x & 0xfe00;
  u16 yAttr = y & 0xff00;

  x &= 0x1ff;
  y = (y - 32) & 0xff;
  if (mirror) {
    x |= OBJ_X_FLIP;
  }
  Sprite_Queue(list, x | xAttr, y | yAttr, data_02151d38, attr, 0);
}
