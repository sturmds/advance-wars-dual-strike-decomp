#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Needed to match: enum results put moveq before movne. */
enum { SAME_TEAM, OTHER_TEAM };

/* TRUE when tile (x, y) holds a unit whose owner is on another team than
 * the current player. */
int Tile_HasEnemyUnit(int x, int y, int arg2) {
  u8 slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return STATE->players[STATE->currentPlayer].team ==
                 STATE->players[(slot >> 6) + 1].team
             ? SAME_TEAM
             : OTHER_TEAM;
}
