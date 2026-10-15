#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern s8 Ov000_gUrbanTerrain
    []; /* nonzero for urban terrain, indexed by terrain id */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit stands on urban terrain. */
BOOL Unit_IsOnUrbanTerrain(MapUnit* unit) {
  return Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(
             MAP_STATE->mapTiles
                 ->tiles[unit->x + MAP_STATE->mapTiles->rowStart[unit->y]])] !=
         0;
}
