#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 when a foot unit (Infantry or Mech) of the current player has waited:
 * counts the foot units, then takes off the ones that have not waited. */
BOOL Ov001_CurrentPlayerHasWaitedFootUnit(void) {
  int count = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE) {
      if (unit->type <= UNIT_MECH) {
        count++;
      }
      if ((unit->flags & MAP_UNIT_WAITED) == 0) {
        if (unit->type <= UNIT_MECH) {
          count--;
        }
      }
    }
  }
  return count != 0;
}
