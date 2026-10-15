#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/ai/ai_unload.h"

/* Tile test for AiSeaTransport_MoveLoaded's order 8-10 routine: the range
 * map is likely the one filled for the carried unit (guess). TRUE when the transport's own
 * target is not reachable and the unit in cargo slot 0 reaches its target. */
BOOL AiSeaTransport_CargoReachesTarget(void) {
  MapUnit* carried = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  return MAP_STATE->rangeMap[carried->aiTargetY][carried->aiTargetX] > 0;
}
