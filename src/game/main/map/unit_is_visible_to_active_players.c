#include "types.h"
#include "game/Unk_02080440.h"

BOOL Tile_HasActivePlayerUnit(
    s16 x, s16 y); /* needed to match: s16, though the definition takes int */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit at tile (x, y) can be shown. Same shape as
 * Unit_IsVisibleToCurrentPlayer, but tests Player.visionGrids bit 1 (doc 890: the
 * active player) where that one compares teams. Only dived units (Sub,
 * Stealth) can be hidden; they stay seen when their owner or the owner of
 * their tile has the bit, or when Tile_HasActivePlayerUnit finds a unit next to them. */
BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, s16 x, s16 y) {
  u16 slot = unit - MAP_STATE->units;
  u16 count = 0;

  /* the original has this null test after it used the pointer */
  if (unit == NULL) {
    return TRUE;
  }
  if (!(unit->flags & MAP_UNIT_DIVED)) {
    return TRUE;
  }
  if (MAP_STATE->players[(slot >> 6) + 1].visionGrids & 2) {
    return TRUE;
  }
  if (MAP_STATE
          ->players[MAP_TILE_OWNER(
              MAP_TILES->tiles[x + MAP_TILES->rowStart[y]])]
          .visionGrids &
      2) {
    return TRUE;
  }
  if (x > 0) {
    count += Tile_HasActivePlayerUnit(x - 1, y);
  }
  if (y > 0) {
    count += Tile_HasActivePlayerUnit(x, y - 1);
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    count += Tile_HasActivePlayerUnit(x + 1, y);
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    count += Tile_HasActivePlayerUnit(x, y + 1);
  }
  return count != 0;
}
