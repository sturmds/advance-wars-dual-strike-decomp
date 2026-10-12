#include "types.h"
#include "game/map_unit.h"

/* Clears the AI fields of a unit: no target, no service or transport wanted,
 * no landmass claim, and three bits of unknown meaning (unk_08_7, unk_0c_5,
 * unk_0c_6). */
void MapUnit_ResetAiState(MapUnit* unit) {
  unit->aiTargetX = MAP_UNIT_NO_TARGET;
  unit->aiTargetY = MAP_UNIT_NO_TARGET;
  unit->serviceWanted = 0;
  unit->transportWanted = 0;
  unit->unk_08_7 = 0;
  unit->unk_0c_5 = 0;
  unit->unk_0c_6 = 0;
  unit->claimedLandmass = 0;
}
