#include "types.h"

extern u8 data_02151d10[];

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Sprite_QueueDefault with a tile number. */
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list) {
  Sprite_Queue(list, x | (u16)(x & 0xfe00), y | (u16)(y & 0xff00),
               data_02151d10, attr, tile);
}
