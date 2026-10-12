#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gUrbanTerrain
    [32]; /* nonzero for urban terrain, indexed by terrain id */

int Unit_IsTileOwnedByTeam(int unitIndex, int tile);

/* Nonzero when tile (x, y) is a capturable property (urban terrain) that the
 * acting unit's team does not own. */
int Map_IsCapturableByActingUnit(s16 x, s16 y) {
  int tile = data_027c027c->unk_48[data_027c0284]
                 ->mapTiles->tiles[x + data_027c027c->unk_48[data_027c0284]
                                           ->mapTiles->rowStart[y]];

  /* needed to match: the mask keeps the tile a byte */
  if (Unit_IsTileOwnedByTeam(
          data_027c027c->unk_48[data_027c0284]->actingUnitIndex, tile & 0xff) ==
      1) {
    return 0;
  }
  return Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(tile)];
}
