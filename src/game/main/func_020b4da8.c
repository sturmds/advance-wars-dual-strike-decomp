#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai.h"

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

/* Fills the range map from (x, y) for the unit that the acting unit
 * carries in its second cargo slot, then runs AiSeaTransport_CargoReachesHq. */
void func_020b4da8(int x, int y) {
  MapUnit* units = MAP_STATE->units;
  int slot = MAP_STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  AiSeaTransport_CargoReachesHq();
}
