#include "types.h"
#include "game/oam_cells.h"

u32 HwDiv32(int num, int denom);
u32 HwMod32(int num, int denom);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

void func_020485f0(u32 pages) {
  u8 x = 220;
  u8 digit;
  u8 rest;

  if (pages <= 1) {
    return;
  }
  Sprite_QueueAt(4, x, 10, &data_027c0060, 0x6800, 0x19a);
  while (pages != 0) {
    x -= 8;
    digit = HwMod32(pages, 10);
    rest = HwDiv32(pages, 10);
    Sprite_QueueAt(4, x & 0x1ff, 10, &data_027c0038, 0x6800, 0x186 + digit * 2);
    pages = rest;
  }
}
