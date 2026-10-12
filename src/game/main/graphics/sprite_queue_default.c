#include "types.h"

extern u8 data_02151d10[];

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Queues a sprite that always uses the data at data_02151d10. Ors the
 * attribute bits above the position back into x and y (needed to match). */
void Sprite_QueueDefault(int x, int y, int attr, int list) {
  Sprite_Queue(list, x | (u16)(x & 0xfe00), y | (u16)(y & 0xff00),
               data_02151d10, attr, 0);
}
