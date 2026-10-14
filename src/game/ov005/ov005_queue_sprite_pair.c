#include "types.h"
#include "game/oam_cells.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Queues two sprites 128 pixels apart, the second 32 tiles after the first.
 * An offscreen y is replaced by 192. `flags` goes to bits 10 and up of the
 * y word. */
void Ov005_QueueSpritePair(void* work, int x, int y, int tile, u16 flags) {
  int attrY;

  if (y >= 192) {
    y = 192;
  }
  if (y < -16) {
    y = 192;
  }
  attrY = (y & 0xff) | (flags << 10);
  Sprite_Queue(0, x & 0x1ff, attrY, &data_027c01c0, 0x400, tile);
  Sprite_Queue(0, (x + 128) & 0x1ff, attrY, &data_027c01c0, 0x400, tile + 32);
}
