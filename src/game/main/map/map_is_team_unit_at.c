#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when the tile is in sight and holds a unit (MapTiles.unk_508 slot) on the
 * current player's team. A unit not in unitAt must have MAP_UNIT_LOCKED set. Used
 * to find allied units next to a dived unit. */
BOOL Map_IsTeamUnitAt(int x, int y) {
  int tile = x + MAP_STATE->mapTiles->rowStart[y];
  int slot = MAP_STATE->mapTiles->unk_508[tile];

  if (slot == 0 || MAP_STATE->mapTiles->inSight[tile] == 0) {
    return FALSE;
  }
  if (MAP_STATE->mapTiles->unitAt[tile] == 0 &&
      !(MAP_STATE->units[slot].flags & MAP_UNIT_LOCKED)) {
    return FALSE;
  }
  return MAP_STATE->players[MAP_STATE->currentPlayer].team ==
         MAP_STATE->players[(slot >> 6) + 1].team;
}
