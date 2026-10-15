#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

#define UNIT(i) (MAP_STATE->units[(i) + MAP_STATE->currentUnitBase])

/* The L button: the current player's next unit that has not waited this turn,
 * looking at slots 1-50 after cycleUnitIndex, then wrapping round to 1. NULL
 * when there is none. cycleUnitIndex goes back to 0 after slot 50. */
MapUnit* CurrentPlayer_NextIdleUnit(void) {
  u16 i;

  for (i = MAP_STATE->cycleUnitIndex + 1; i < MAP_UNITS_MAX + 1; i++) {
    if (UNIT(i).type != 0 && !(UNIT(i).flags & MAP_UNIT_WAITED)) {
      MAP_STATE->cycleUnitIndex = i;
      if (MAP_STATE->cycleUnitIndex >= MAP_UNITS_MAX) {
        MAP_STATE->cycleUnitIndex = 0;
      }
      return &UNIT(i);
    }
  }
  for (i = 1; i < MAP_STATE->cycleUnitIndex + 1; i++) {
    if (UNIT(i).type != 0 && !(UNIT(i).flags & MAP_UNIT_WAITED)) {
      MAP_STATE->cycleUnitIndex = i;
      if (MAP_STATE->cycleUnitIndex >= MAP_UNITS_MAX) {
        MAP_STATE->cycleUnitIndex = 0;
      }
      return &UNIT(i);
    }
  }
  return NULL;
}
