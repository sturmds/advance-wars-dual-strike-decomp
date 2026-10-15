#include "types.h"
#include "game/Unk_02080440.h"

/* Per unit type: the terrain id where an owned tile spares the unit its daily
 * fuel. Airport for planes and copters, port for ships, 0 (none) for ground
 * units. */
extern u8 Ov000_gFuelFreeTerrain[];

/* tileHp bit 7: within range of a Black Crystal or Obelisk
 * (Map_MarkCrystalObeliskRange) */
#define TILE_HP_FLAG 0x80

/* FALSE when the unit uses no fuel at the start of the day: while carried, on a
 * tile of Ov000_gFuelFreeTerrain owned by the current player, or as a Black
 * Hole unit on a tile marked by a Black Crystal or Obelisk. */
BOOL Unit_UsesDailyFuel(MapUnit* unit) {
  int tileIndex = unit->x + MAP_TILES->rowStart[unit->y];
  u8 tile = MAP_TILES->tiles[tileIndex];

  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  if ((tile & 0xe0) == MAP_STATE->unk_04 &&
      Ov000_gFuelFreeTerrain[unit->type] == MAP_TILE_TERRAIN(tile)) {
    return FALSE;
  }
  if (MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unit)].armyColour ==
          ARMY_BLACK_HOLE &&
      (MAP_STATE->tileHp[tileIndex] & TILE_HP_FLAG)) {
    return FALSE;
  }
  return TRUE;
}
