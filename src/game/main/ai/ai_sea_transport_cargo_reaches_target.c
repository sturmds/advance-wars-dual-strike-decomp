#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL AiUnit_HasReachableTarget(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Tile test for AiSeaTransport_MoveLoaded's order 8-10 routine: the range
 * map is likely the one filled for the carried unit (guess). TRUE when the transport's own
 * target is not reachable and the unit in cargo slot 0 reaches its target. */
BOOL AiSeaTransport_CargoReachesTarget(void) {
  MapUnit* carried = &STATE->units[STATE->actingUnit->cargo[0]];

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  return STATE->rangeMap[carried->aiTargetY][carried->aiTargetX] > 0;
}
