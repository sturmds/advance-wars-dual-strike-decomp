#include "types.h"
#include "game/Unk_0229c734.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

#define SCORES data_0229c734

/* Queues the bar under the score rows with its top left at (x, y): four
 * sprites 32 pixels apart and a fifth 16 pixels short of the end. */
void ResultPoint_DrawBar(int x, int y) {
  int i;
  int spriteX = x;

  for (i = 0; i < 4; i++) {
    Sprite_QueueAt(6, spriteX & 0x1ff, y & 0xff, &data_027c0020,
                   SCORES->paletteSlot << 12, SCORES->partTile[3] + 20);
    spriteX += 32;
  }
  Sprite_QueueAt(6, (x + i * 32 - 16) & 0x1ff, y & 0xff, &data_027c0020,
                 SCORES->paletteSlot << 12, SCORES->partTile[3] + 20);
}
