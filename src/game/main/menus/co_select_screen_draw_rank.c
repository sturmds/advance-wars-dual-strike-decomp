#include "types.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the rank label at (x, y) and the rank after it as one or two
 * digits from x + 32, like ForceRankScreen_DrawRank. With two digits the
 * ones digit sits 7 pixels after the tens for tens 1-3 and 8 for larger
 * ones. Rank 100 gets one sprite of its own instead of digits. */
void CoSelectScreen_DrawRank(int x, int y, u8 rank) {
  int tens;
  int onesGap = 0;

  Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0060, 0x3000, 888);
  if (rank == 100) {
    Sprite_QueueAt(1, (x + 32) & 0x1ff, y & 0xff, &data_027c0040, 0x3000, 916);
    return;
  }
  if (rank >= 10) {
    tens = HwDiv32(rank, 10);
    rank = HwMod32(rank, 10);
    Sprite_QueueAt(1, (x + 32) & 0x1ff, y & 0xff, &data_027c0038, 0x3000,
                   tens * 2 + 896);
    if (tens - 1 <= 2u) { /* tens 1-3; this form needed to match */
      onesGap = 7;
    } else {
      onesGap = 8;
    }
  }
  Sprite_QueueAt(1, (x + 32 + onesGap) & 0x1ff, y & 0xff, &data_027c0038,
                 0x3000, rank * 2 + 896);
}
