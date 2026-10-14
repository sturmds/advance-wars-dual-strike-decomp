#include "types.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues the rank label at (x, y) and the rank after it as one or two
 * digits from x + 32. Rank 100 gets one sprite of its own instead of
 * digits. ForceRankScreen_Draw passes ForceRankScreen.rank. */
void ForceRankScreen_DrawRank(int x, int y, u8 rank) {
  int tens;

  Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0060, 0x200c, 0);
  if (rank == 100) {
    Sprite_QueueAt(1, (x + 32) & 0x1ff, y & 0xff, &data_027c0040, 0x2028, 0);
    return;
  }
  if (rank >= 10) {
    tens = HwDiv32(rank, 10);
    rank = HwMod32(rank, 10);
    Sprite_QueueAt(1, (x + 32) & 0x1ff, y & 0xff, &data_027c0038,
                   (tens * 2 + 20) | 0x2000, 0);
  }
  Sprite_QueueAt(1, (x + 40) & 0x1ff, y & 0xff, &data_027c0038,
                 (rank * 2 + 20) | 0x2000, 0);
}
