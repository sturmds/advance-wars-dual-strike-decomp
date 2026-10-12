#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
int Map_IsTeamUnitAt(s16 x, s16 y);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAP (GAME->mapTiles)

/* TRUE when the current player can see the unit in slot unitSlot. Only dived
 * units (Sub, Stealth) can be hidden. They stay seen when they belong to the
 * current player's team, when their tile is owned by that team, or when
 * Map_IsTeamUnitAt finds something on a neighbouring tile (it is not understood
 * yet; its count is only tested against 0). */
BOOL Unit_IsVisibleToCurrentPlayer(int unitSlot) {
  MapUnit* unit = &GAME->units[unitSlot];
  s16 x = unit->x;
  s16 y = unit->y;
  u16 count = 0;

  /* the original has this null test (it cannot fail for a real slot) */
  if (unit == NULL) {
    return TRUE;
  }
  if (!(unit->flags & MAP_UNIT_DIVED)) {
    return TRUE;
  }
  if (GAME->players[GAME->currentPlayer].team ==
      GAME->players[MAP_UNIT_OWNER(GAME, unit)].team) {
    return TRUE;
  }
  /* needed to match: the original reads the slot table as s16 */
  if (Unit_IsTileOwnedByTeam(
          ((s16*)Ov000_gPlayerFirstUnitSlot)[GAME->currentPlayer],
          MAP->tiles[x + MAP->rowStart[y]])) {
    return TRUE;
  }
  if (x > 0) {
    count += Map_IsTeamUnitAt(x - 1, y);
  }
  if (y > 0) {
    count += Map_IsTeamUnitAt(x, y - 1);
  }
  if (x < GAME->unk_1c4->mapWidth - 1) {
    count += Map_IsTeamUnitAt(x + 1, y);
  }
  if (y < GAME->unk_1c4->mapHeight - 1) {
    count += Map_IsTeamUnitAt(x, y + 1);
  }
  return count != 0;
}
