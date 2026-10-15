#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

void Players_AssignVisionGrids(void);

/* Sets the fields derived from currentPlayer: the tile owner bits (unk_04),
 * the first unit slot, and unk_1e, then assigns the vision grids. */
void CurrentPlayer_SyncState(void) {
  MAP_STATE->unk_04 = MAP_STATE->currentPlayer << 5;
  MAP_STATE->currentUnitBase =
      Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer];
  MAP_STATE->unk_1e = MAP_STATE->currentPlayer;
  Players_AssignVisionGrids();
}
