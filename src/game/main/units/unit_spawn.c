#include "types.h"
#include "game/map_unit.h"

MapUnit* Unit_Create(int x, int y, int type);
void func_020c8888(void);

/* Creates a unit for the current player (Unit_Create) and refreshes the map
 * state for it. NULL when the player has no free unit slot. */
MapUnit* Unit_Spawn(int x, int y, int type) {
  MapUnit* unit = Unit_Create(x, y, type);

  if (unit == NULL) {
    return NULL;
  }
  func_020c8888();
  return unit;
}
