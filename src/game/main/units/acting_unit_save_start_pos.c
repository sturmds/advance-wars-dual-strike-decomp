#include "types.h"
#include "game/Unk_02080440.h"
#include "game/acting_unit.h"

/* Saves the tile the acting unit started its move on. */
void ActingUnit_SaveStartPos(void) {
  data_0229ead4[0] = MAP_STATE->actingUnitStartX;
  data_0229ead4[1] = MAP_STATE->actingUnitStartY;
}
