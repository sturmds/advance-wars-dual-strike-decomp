#include "types.h"
#include "game/oam_cells.h"
#include "game/math.h"

void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

/* Draws value (0-999) as digit sprites 8 pixels apart from x, without leading
 * zeros; digit tiles are 2 tiles wide from tile 0x1b6. */
void MapSelect_DrawNumber(int x, int y, int value) {
  int digit;
  int printed = 0;

  if (value >= 100) {
    digit = HwDiv32(value, 100);
    value = HwMod32(value, 100);
    Sprite_QueueAt(0, x & 0x1ff, y & 0xff, &data_027c0038, 0x1000,
                   0x1b6 + digit * 2);
    printed = 1;
  }
  if (printed || value >= 10) {
    digit = HwDiv32(value, 10);
    value = HwMod32(value, 10);
    Sprite_QueueAt(0, (x + 8) & 0x1ff, y & 0xff, &data_027c0038, 0x1000,
                   0x1b6 + digit * 2);
  }
  Sprite_QueueAt(0, (x + 16) & 0x1ff, y & 0xff, &data_027c0038, 0x1000,
                 0x1b6 + value * 2);
}
