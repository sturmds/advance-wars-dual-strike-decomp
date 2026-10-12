#include "types.h"

extern u8 data_02166310[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Like Oam_PutSmallNumber, but queues the digit sprites on a sprite list. The
 * digit goes in Sprite_QueueAt's attr argument. */
void Sprite_QueueSmallNumber(int list, u16 x, int y, u32 value) {
  for (;;) {
    Sprite_QueueAt(list, x & 0x1ff, y & 0xff, data_02166310, value % 10, 0);
    value /= 10;
    if (value == 0) {
      break;
    }
    x -= 7;
  }
}
