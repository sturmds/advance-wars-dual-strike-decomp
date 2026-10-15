#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Removes every unit of the four players by clearing its type, and resets
 * the unit that CurrentPlayer_NextIdleUnit last jumped to. */
void Units_ClearAll(void) {
  int i;

  for (i = 0; i < 4 * MAP_UNITS_PER_PLAYER; i++) {
    MAP_STATE->units[i].type = UNIT_NONE;
  }
  MAP_STATE->cycleUnitIndex = 0;
}
