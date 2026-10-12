#include "types.h"
#include "game/Unk_0229c734.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void ResultPoint_QueueNumber(int x, int y, int value, int arg3);

extern u8 data_027c0060[];
extern u8 data_027c01ac[];

#define SCORES data_0229c734

/* Queues the points panel at (x, y): a frame sprite, the number `points`
 * 80 pixels in and a label sprite 120 pixels in, all 8 pixels lower. */
void ResultPoint_DrawPanel(int x, int y, int points) {
  /* needed to match: y moved down once, cast to u8 at each sprite call */
  y += 8;
  Sprite_QueueAt(6, x & 0x1ff, (u8)y, data_027c01ac,
                 (SCORES->paletteSlot + 1) << 12, SCORES->partTile[3]);
  ResultPoint_QueueNumber(x + 80, y, points, 1);
  Sprite_QueueAt(6, (x + 120) & 0x1ff, (u8)y, data_027c0060,
                 (SCORES->paletteSlot + 1) << 12, SCORES->partTile[3] + 72);
}
