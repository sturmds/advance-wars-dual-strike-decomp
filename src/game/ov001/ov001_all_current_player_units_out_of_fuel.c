#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 when every unit of the current player has no fuel left. */
BOOL Ov001_AllCurrentPlayerUnitsOutOfFuel(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && unit->fuel != 0) {
      return 0;
    }
  }
  return 1;
}
