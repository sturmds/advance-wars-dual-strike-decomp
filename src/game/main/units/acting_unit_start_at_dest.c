#include "types.h"
#include "game/Unk_02080440.h"

/* Sets the acting unit's start tile to its destination, so it counts as not moved. */
void ActingUnit_StartAtDest(void) {
  MAP_STATE->actingUnitStartX = MAP_STATE->actingUnitDestX;
  MAP_STATE->actingUnitStartY = MAP_STATE->actingUnitDestY;
}
