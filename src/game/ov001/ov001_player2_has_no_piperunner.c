#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* 1 when player 2 has no Piperunner among its 50 unit slots (the scan skips the first
 * slot, like the AI queue scans). */
BOOL Ov001_Player2HasNoPiperunner(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[2] + 1;
       i < Ov000_gPlayerFirstUnitSlot[2] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type == UNIT_PIPERUNNER) {
      return 0;
    }
  }
  return 1;
}
