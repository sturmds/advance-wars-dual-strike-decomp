#include "types.h"

extern u8 data_02151ccc[];

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Queues the data_02151ccc sprite on list at (x, y). x keeps its OAM attribute bits above bit 8,
 * y keeps the bits above bit 7 and is moved up 16 pixels. */
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile,
                                      int list) {
  u16 xHigh = x & 0xfe00;
  u16 yHigh = y & 0xff00;

  y = (y - 16) & 0xff;
  x &= 0x1ff;
  Sprite_Queue(list, x | xHigh, y | yHigh, data_02151ccc, attr, tile);
}
