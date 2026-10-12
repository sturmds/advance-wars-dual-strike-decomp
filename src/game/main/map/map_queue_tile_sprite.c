#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues a sprite on list 4 at map tile (x, y), placed against the current map scroll. x and y are reused
 * for the screen position (needed to match). Callers pass 0 as unused. */
void Map_QueueTileSprite(int x, int y, void* data, int palette, int unused) {
  x = x * 16 - STATE->unk_1c4->scrollX;
  y = y * 16 - STATE->unk_1c4->scrollY;
  Sprite_QueueAt(4, x, y, data, (palette << 12) + 0x94, 0);
}
