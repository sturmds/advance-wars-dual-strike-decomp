#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Tile test for AiTransport_SecondCargoHasTaskAt's order 8-10 routine: fills
 * the range map from (x, y) for the unit in the acting transport's second
 * cargo slot. TRUE when that unit has a target and the map reaches it. */
BOOL AiTransport_SecondCargoReachesTargetAt(int x, int y) {
  MapUnit* units = STATE->units;
  int slot = STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  if (carried->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  return STATE->rangeMap[carried->aiTargetY][carried->aiTargetX] > 0;
}
