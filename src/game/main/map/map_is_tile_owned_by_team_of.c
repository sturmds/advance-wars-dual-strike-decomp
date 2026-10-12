#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* needed to match: the original adds the scaled index to the base pointer
 * and loads the index first. */
#define STATE_AT(index)                                             \
  (*(Unk_02080440_48**)((u8*)((index) * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &            \
                        ((Unk_02080440*)0)->unk_48))

/* TRUE when tile (x, y) has an owner on the same team as player. Neutral
 * tiles give FALSE. */
BOOL Map_IsTileOwnedByTeamOf(int player, int x, int y) {
  Unk_02080440_48* state = STATE_AT(data_027c0284);
  int owner = state->mapTiles->tiles[x + state->mapTiles->rowStart[y]] >> 5;

  if (owner == 0) {
    return FALSE;
  }
  return state->players[player].team == state->players[owner].team;
}
