#include "types.h"

void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile,
                                      int list);
BOOL Sprite_IsOnScreenX(int x, int width);

/* Queues the sprite of CO slot `slot` (two per player) at (x + 24, y + 16)
 * from tile 174, 12 tiles per slot; skipped when off screen. Unknown
 * whether it shows the CO name or a picture, so it has no name yet. */
void func_0203be58(int x, int y, int slot) {
  if (!Sprite_IsOnScreenX(x, 48)) {
    return;
  }
  Co_QueueSmallPictureSpriteAtTile((x + 24) & 0x1ff, (y + 16) & 0xff,
                                   (slot + 2) << 12, 174 + slot * 12, 0);
}
