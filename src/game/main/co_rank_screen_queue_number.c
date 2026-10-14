#include "types.h"
#include "game/Unk_022a7b80.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern Unk_022a7b80 data_022a7b80;

/* Queues a label sprite at (x + 8, y), picked by the mode in
 * data_022a7b80.mode (0-2 or 3-5), then `value` as up to four digits
 * at x + 104 with leading zeros left out, then a suffix sprite at x + 136. */
void CoRankScreen_QueueNumber(int x, int y, int value) {
  int digit;
  int drawn;

  drawn = 0;
  switch (data_022a7b80.mode) {
    case 0:
    case 1:
    case 2:
      Sprite_QueueAt(7, (x + 8) & 0x1ff, y & 0xff, &data_027c0148, 0x3000,
                     0x144);
      break;
    default:
      Sprite_QueueAt(7, (x + 8) & 0x1ff, y & 0xff, &data_027c0148, 0x3000,
                     0x15c);
      break;
  }
  digit = HwDiv32(value, 1000);
  if (digit != 0) {
    drawn = 1;
    Sprite_QueueAt(7, (x + 104) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x4000,
                   200 + digit * 2);
  }
  value -= digit * 1000;
  digit = HwDiv32(value, 100);
  if (drawn == 1 || digit != 0) {
    drawn = 1;
    Sprite_QueueAt(7, (x + 112) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x4000,
                   200 + digit * 2);
  }
  value -= digit * 100;
  digit = HwDiv32(value, 10);
  if (drawn == 1 || digit != 0) {
    Sprite_QueueAt(7, (x + 120) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x4000,
                   200 + digit * 2);
  }
  value -= digit * 10;
  Sprite_QueueAt(7, (x + 128) & 0x1ff, (y + 1) & 0xff, &data_027c0038, 0x4000,
                 200 + value * 2);
  Sprite_QueueAt(7, (x + 136) & 0x1ff, (y + 7) & 0xff, &data_027c0020, 0x4000,
                 0xe0);
}
