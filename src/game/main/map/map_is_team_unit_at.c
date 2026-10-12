#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the tile is in sight and holds a unit (MapTiles.unk_508 slot) on the
 * current player's team. A unit not in unitAt must have MAP_UNIT_LOCKED set. Used
 * to find allied units next to a dived unit. */
BOOL Map_IsTeamUnitAt(int x, int y) {
  int tile = x + STATE->mapTiles->rowStart[y];
  int slot = STATE->mapTiles->unk_508[tile];

  if (slot == 0 || STATE->mapTiles->inSight[tile] == 0) {
    return FALSE;
  }
  if (STATE->mapTiles->unitAt[tile] == 0 &&
      !(STATE->units[slot].flags & MAP_UNIT_LOCKED)) {
    return FALSE;
  }
  return STATE->players[STATE->currentPlayer].team ==
         STATE->players[(slot >> 6) + 1].team;
}
