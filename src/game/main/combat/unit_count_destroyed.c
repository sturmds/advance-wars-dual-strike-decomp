#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

/* gPlayRecords + 0x37c. Needed to match: the original indexes from here
 * (type + 77) instead of from PlayRecords.unitsDestroyed. */
extern u32 data_02290a94[];

#define UNITS_DESTROYED_OFFSET 77

/* Counts the unit's type as destroyed in the play records (cap 999999),
 * then does the same for each unit it carries. */
void Unit_CountDestroyed(MapUnit* unit) {
  if (data_02290a94[unit->type + UNITS_DESTROYED_OFFSET] < 999999) {
    data_02290a94[unit->type + UNITS_DESTROYED_OFFSET]++;
  }
  if (unit->cargo[0] != 0) {
    Unit_CountDestroyed(&MAP_STATE->units[unit->cargo[0]]);
  }
  if (unit->cargo[1] != 0) {
    Unit_CountDestroyed(&MAP_STATE->units[unit->cargo[1]]);
  }
}
