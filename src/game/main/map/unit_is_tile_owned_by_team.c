#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when map tile byte tile has an owner on the same team as the player
 * who owns unit slot unitIndex. Neutral tiles give FALSE. */
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile) {
  u8 owner = tile >> 5;

  if (owner == 0) {
    return FALSE;
  }
  return STATE->players[(unitIndex >> 6) + 1].team ==
         STATE->players[owner].team;
}
