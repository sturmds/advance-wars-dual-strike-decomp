#include "types.h"

void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);
BOOL Sprite_IsOnScreenX(int x, int width);

/* Queues the sprite of CO slot `slot` (two per player) at (x, y) from
 * tile 270, 12 tiles per slot; skipped when off screen. Unknown whether it
 * shows the CO name or a picture, so the name stays an address. */
void func_0203bdf0(int x, int y, int slot) {
  if (!Sprite_IsOnScreenX(x, 48)) {
    return;
  }
  Sprite_QueueDefaultTile(x & 0x1ff, y & 0xff, 0xa000, 270 + slot * 12, 0);
}
