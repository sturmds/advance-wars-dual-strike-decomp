#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when map tile byte tile has an owner on the same team as the player
 * who owns unit slot unitIndex. Neutral tiles give FALSE. */
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile) {
  u8 owner = tile >> 5;

  if (owner == 0) {
    return FALSE;
  }
  return MAP_STATE->players[(unitIndex >> 6) + 1].team ==
         MAP_STATE->players[owner].team;
}
