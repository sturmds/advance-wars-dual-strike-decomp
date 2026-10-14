#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gUrbanTerrain
    []; /* nonzero for urban terrain, indexed by terrain id */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit stands on urban terrain. */
BOOL Unit_IsOnUrbanTerrain(MapUnit* unit) {
  return Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(
             STATE->mapTiles
                 ->tiles[unit->x + STATE->mapTiles->rowStart[unit->y]])] != 0;
}
