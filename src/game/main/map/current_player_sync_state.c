#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

void Players_AssignVisionGrids(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Sets the fields derived from currentPlayer: the tile owner bits (unk_04),
 * the first unit slot, and unk_1e, then assigns the vision grids. */
void CurrentPlayer_SyncState(void) {
  GAME->unk_04 = GAME->currentPlayer << 5;
  GAME->currentUnitBase = Ov000_gPlayerFirstUnitSlot[GAME->currentPlayer];
  GAME->unk_1e = GAME->currentPlayer;
  Players_AssignVisionGrids();
}
