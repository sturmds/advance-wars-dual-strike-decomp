#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when tile (x, y) has an owner on a team other than that of the
 * player who owns unit slot unitIndex. Neutral tiles give FALSE. */
BOOL Unit_IsEnemyTile(int unitIndex, int x, int y) {
  u8 owner = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] >> 5;

  if (owner == 0) {
    return FALSE;
  }
  return STATE->players[(unitIndex >> 6) + 1].team !=
         STATE->players[owner].team;
}
