#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when tile (x, y) has an owner on a team other than that of the
 * player who owns unit slot unitIndex. Neutral tiles give FALSE. */
BOOL Unit_IsEnemyTile(int unitIndex, s16 x, s16 y) {
  u8 owner =
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] >> 5;

  if (owner == 0) {
    return FALSE;
  }
  return MAP_STATE->players[(unitIndex >> 6) + 1].team !=
         MAP_STATE->players[owner].team;
}
