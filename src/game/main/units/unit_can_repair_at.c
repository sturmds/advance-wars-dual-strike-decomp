#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Terrain_IsProperty(
    u8 tile); /* the tile's terrain is a property (gTerrainIsProperty) */
BOOL UnitSlot_OwnsTile(u8 slot, s16 x,
                       s16 y); /* the owner of unit slot owns tile (x, y) */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) (STATE->mapTiles->rowStart[y] + (x))

/* TRUE when tile (x, y) is a property of the unit's owner where its type
 * is repaired. The owner comes from MapTiles.unk_508 at the unit's own
 * tile. */
int Unit_CanRepairAt(MapUnit* unit, int x, int y) {
  if (Terrain_IsProperty(STATE->mapTiles->tiles[TILE_INDEX(x, y)]) == TRUE &&
      UnitSlot_OwnsTile(STATE->mapTiles->unk_508[TILE_INDEX(unit->x, unit->y)],
                        x, y) == TRUE &&
      Ov000_gUnitStats[unit->type].repairTerrain[MAP_TILE_TERRAIN(
          STATE->mapTiles->tiles[TILE_INDEX(x, y)])] != 0) {
    return TRUE;
  }
  return FALSE;
}
