#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* gPlayRecords + 0x37c. Needed to match: the original indexes from here
 * (type + 77) instead of from PlayRecords.unitsDestroyed. */
extern u32 data_02290a94[];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define UNITS_DESTROYED_OFFSET 77

/* Counts the unit's type as destroyed in the play records (cap 999999),
 * then does the same for each unit it carries. */
void Unit_CountDestroyed(MapUnit* unit) {
  if (data_02290a94[unit->type + UNITS_DESTROYED_OFFSET] < 999999) {
    data_02290a94[unit->type + UNITS_DESTROYED_OFFSET]++;
  }
  if (unit->cargo[0] != 0) {
    Unit_CountDestroyed(&STATE->units[unit->cargo[0]]);
  }
  if (unit->cargo[1] != 0) {
    Unit_CountDestroyed(&STATE->units[unit->cargo[1]]);
  }
}
