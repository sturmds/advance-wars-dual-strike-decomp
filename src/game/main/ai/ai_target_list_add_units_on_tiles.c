#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiTargetList_Add(u16 a, u16 b, u16 unitSlot, u16 c);

/* Adds the unit standing on each tile of the list (ended by x == 0xffff) to
 * the AI target list. */
void AiTargetList_AddUnitsOnTiles(TilePointU16* points, int extra) {
  for (; points->x != TILE_POINT_END; points++) {
    MapTiles* tiles = data_027c027c->unk_48[data_027c0284]->mapTiles;
    u8 slot = tiles->unitAt[points->x + tiles->rowStart[points->y]];

    if (slot != 0) {
      AiTargetList_Add(points->x, points->y, slot, extra);
    }
  }
}
