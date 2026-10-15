#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* Returns FALSE when the current player has a unit that has not waited yet
 * and is a Lander, B Copter, Battleship, Cruiser or Sub; TRUE otherwise. */
BOOL Ov001_HasNoReadyShipOrBCopter(void) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    MapUnit* unit = &MAP_STATE->units[slot];

    if (unit->type != UNIT_NONE && !(unit->flags & MAP_UNIT_WAITED)) {
      if (unit->type == UNIT_LANDER) {
        return FALSE;
      }
      if (unit->type == UNIT_B_COPTER) {
        return FALSE;
      }
      if (unit->type == UNIT_BATTLESHIP) {
        return FALSE;
      }
      if (unit->type == UNIT_CRUISER) {
        return FALSE;
      }
      if (unit->type == UNIT_SUB) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
