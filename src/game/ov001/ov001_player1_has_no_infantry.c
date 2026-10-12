#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 when player 1 has no Infantry among its 50 unit slots (the scan skips the first
 * slot, like the AI queue scans). */
BOOL Ov001_Player1HasNoInfantry(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[1] + 1;
       i < Ov000_gPlayerFirstUnitSlot[1] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type == UNIT_INFANTRY) {
      return 0;
    }
  }
  return 1;
}
