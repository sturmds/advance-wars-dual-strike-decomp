#include "types.h"
#include "game/map_unit.h"

MapUnit* Unit_Create(int x, int y, int type);
void func_020c8888(void);

/* Like Unit_Spawn, but the new unit starts out waited (it cannot act this
 * turn): units bought at a base and units created by map events. */
MapUnit* Unit_SpawnWaited(int x, int y, int type) {
  MapUnit* unit = Unit_Create(x, y, type);

  if (unit == NULL) {
    return NULL;
  }
  unit->flags |= MAP_UNIT_WAITED;
  func_020c8888();
  return unit;
}
