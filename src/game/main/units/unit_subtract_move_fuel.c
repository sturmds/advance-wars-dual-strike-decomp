#include "types.h"
#include "game/map_unit.h"

int Unit_GetFuelUsed(MapUnit* unit, int moveCost);

/* Takes the fuel a move of moveCost movement points uses off the unit. */
void Unit_SubtractMoveFuel(MapUnit* unit, int moveCost) {
  unit->fuel -= Unit_GetFuelUsed(unit, moveCost);
}
