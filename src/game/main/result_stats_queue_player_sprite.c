#include "types.h"

void Sprite_QueueAt(int list, int x, u8 y, void* data, int attr, int tile);
BOOL Sprite_IsOnScreenX(int x, int width);

extern u8 data_027c0060[];

/* Queues the sprite of player `index` (counted from 0) at (x, y), 8 tiles
 * per player; skipped when off screen. */
void ResultStats_QueuePlayerSprite(int x, int y, int index) {
  if (!Sprite_IsOnScreenX(x, 32)) {
    return;
  }
  Sprite_QueueAt(0, x & 0x1ff, y, &data_027c0060, 0, index * 8);
}
