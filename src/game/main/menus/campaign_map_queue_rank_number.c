#include "types.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
int HwDiv32(int num, int den);
int HwMod32(int num, int den);

/* Queues the rank (Co_GetRank) of one top CO on the world map panel: a
 * frame sprite at x, then one sprite at x + 25 for rank 100, otherwise
 * up to two digit sprites at x + 25 and x + 33 (digit d uses cell
 * d * 2 + 4). */
void CampaignMap_QueueRankNumber(int x, int y, int rank) {
  Sprite_QueueAt(0, x & 0x1ff, y & 0xff, &data_027c0020, 0, 0);
  if (rank == 100) {
    Sprite_QueueAt(0, (x + 25) & 0x1ff, y & 0xff, &data_027c0058, 24, 0);
    return;
  }
  if (rank >= 10) {
    Sprite_QueueAt(0, (x + 25) & 0x1ff, y & 0xff, &data_027c0028,
                   HwDiv32(rank, 10) * 2 + 4, 0);
    rank = HwMod32(rank, 10);
  }
  Sprite_QueueAt(0, (x + 33) & 0x1ff, y & 0xff, &data_027c0028, rank * 2 + 4,
                 0);
}
