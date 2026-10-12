#include "types.h"

extern u8 data_02151df8[];
void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Co_QueuePictureSprite with a tile number. */
void Co_QueuePictureSpriteAtTile(int x, int y, int attr, int tile, int list) {
  u16 xHigh = x & 0xfe00;
  u16 yHigh = y & 0xff00;

  y = (y - 48) & 0xff;
  x &= 0x1ff;
  Sprite_Queue(list, x | xHigh, y | yHigh, data_02151df8, attr, tile);
}
