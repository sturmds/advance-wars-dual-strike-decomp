#include "types.h"

extern u8 data_02151ccc[];

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Queues the small CO picture sprite (data_02151ccc) on list at (x, y), tile 0. Callers
 * give attr 12 tiles per slot. */
void Co_QueueSmallPictureSprite(int x, int y, int attr, int list) {
  u16 xHigh = x & 0xfe00;
  u16 yHigh = y & 0xff00;

  y = (y - 16) & 0xff;
  x &= 0x1ff;
  Sprite_Queue(list, x | xHigh, y | yHigh, data_02151ccc, attr, 0);
}
