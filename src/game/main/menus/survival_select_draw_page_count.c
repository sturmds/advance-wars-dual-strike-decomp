#include "types.h"
#include "game/oam_cells.h"

u32 HwDiv32(int num, int denom);
u32 HwMod32(int num, int denom);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

/* Draws the page count of a list with 2 or more pages at the right of the
 * top screen: a marker sprite at x = 220, then the digits of the count
 * from the last one leftwards, 8 pixels apart. */
void SurvivalSelect_DrawPageCount(u32 pages) {
  u8 x = 220;
  u8 digit;
  u8 rest;

  if (pages <= 1) {
    return;
  }
  Sprite_QueueAt(4, x, 10, &data_027c0060, 0x8800, 0x1c2);
  while (pages != 0) {
    x -= 8;
    digit = HwMod32(pages, 10);
    rest = HwDiv32(pages, 10);
    Sprite_QueueAt(4, x & 0x1ff, 10, &data_027c0038, 0x8800, 0x1ae + digit * 2);
    pages = rest;
  }
}
