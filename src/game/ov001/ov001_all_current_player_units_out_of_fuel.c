#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* 1 when every unit of the current player has no fuel left. */
BOOL Ov001_AllCurrentPlayerUnitsOutOfFuel(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->fuel != 0) {
      return 0;
    }
  }
  return 1;
}
