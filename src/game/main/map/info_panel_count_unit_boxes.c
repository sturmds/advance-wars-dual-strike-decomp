#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL UnitType_HidesCargo(int type, int viewer, int owner);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* How many unit boxes the info panel shows for the tile (x, y): 0 for an
 * empty tile, 2 for a unit that carries cargo or whose cargo is hidden from
 * the current player, else 1. */
int InfoPanel_CountUnitBoxes(u8 x, u8 y) {
  Unk_02080440_48* state;
  MapUnit* unit;
  u8 owner;
  u8 slot;

  slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
  if (slot == 0) {
    return 0;
  }
  /* needed to match: STATE is read again for units */
  state = STATE;
  unit = &STATE->units[slot];
  owner = MAP_UNIT_OWNER(state, unit);
  if (UnitType_HidesCargo(unit->type, state->currentPlayer, owner)) {
    return 2;
  }
  if (unit->cargo[0] | unit->cargo[1]) {
    return 2;
  }
  return 1;
}
