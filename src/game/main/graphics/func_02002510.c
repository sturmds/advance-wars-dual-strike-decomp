#include "types.h"

void Sprite_QueueGroup(int list, int x, int y, u16 xBits, u16 yBits,
                       const void* group, u16 tileAdd, u16 attr);

extern u8 data_02151d38[];

#define OBJ_X_FLIP 0x1000

/* Queues the sprite group data_02151d38 (see Sprite_QueueGroup) on sprite list
 * `list`, drawn 32 pixels above y. xBits and yBits are the OAM attribute
 * bits or'd into the group's x and y words; mirror flips the group
 * horizontally. */
void func_02002510(int x, int y, int xBits, int yBits, int tileAdd, int attr,
                   int list, int mirror) {
  u16 xAttr = xBits & 0xfe00;
  u16 yAttr = yBits & 0xff00;

  if (mirror) {
    xAttr |= OBJ_X_FLIP;
  }
  Sprite_QueueGroup(list, x, y - 32, xAttr, yAttr, data_02151d38, tileAdd,
                    attr);
}
