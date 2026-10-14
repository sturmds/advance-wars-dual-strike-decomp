#include "types.h"
#include "game/oam_cells.h"

void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

/* Queues the banner as two 64-pixel sprite halves, centred on x. y is the
 * vertical centre. */
void HistoryBanner_Draw(int x, int y) {
  u8 top = y - 32;

  Oam_PutCells((x - 64) & 0x1ff, top, &data_027c0030, 0x3000, 0x400);
  Oam_PutCells(x & 0x1ff, top, &data_027c0030, 0x3000, 0x440);
}
