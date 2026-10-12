#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Per unit type: the terrain id where an owned tile spares the unit its daily
 * fuel. Airport for planes and copters, port for ships, 0 (none) for ground
 * units. */
extern u8 Ov000_gFuelFreeTerrain[];

/* tileHp bit 7: within range of a Black Crystal or Obelisk
 * (Map_MarkCrystalObeliskRange) */
#define TILE_HP_FLAG 0x80

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAP (GAME->mapTiles)

/* FALSE when the unit uses no fuel at the start of the day: while carried, on a
 * tile of Ov000_gFuelFreeTerrain owned by the current player, or as a Black
 * Hole unit on a tile marked by a Black Crystal or Obelisk. */
BOOL Unit_UsesDailyFuel(MapUnit* unit) {
  int tileIndex = unit->x + MAP->rowStart[unit->y];
  u8 tile = MAP->tiles[tileIndex];

  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  if ((tile & 0xe0) == GAME->unk_04 &&
      Ov000_gFuelFreeTerrain[unit->type] == MAP_TILE_TERRAIN(tile)) {
    return FALSE;
  }
  if (GAME->players[MAP_UNIT_OWNER(GAME, unit)].armyColour == ARMY_BLACK_HOLE &&
      (GAME->tileHp[tileIndex] & TILE_HP_FLAG)) {
    return FALSE;
  }
  return TRUE;
}
