#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 when none of the current player's Rockets or Missiles is still waiting to
 * act and at least one Anti-Air has waited. */
BOOL Ov001_RocketsAndMissilesWaitedAndAntiAirHasWaited(void) {
  int count = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE) {
      if (unit->type == UNIT_ANTI_AIR) {
        count++;
      }
      if ((unit->flags & MAP_UNIT_WAITED) == 0) {
        if (unit->type == UNIT_ROCKETS) {
          return 0;
        }
        if (unit->type == UNIT_MISSILES) {
          return 0;
        }
        if (unit->type == UNIT_ANTI_AIR) {
          count--;
        }
      }
    }
  }
  return count != 0;
}
