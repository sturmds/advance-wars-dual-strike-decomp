#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai.h"

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

BOOL func_020b51ec(int x, int y) {
  MapUnit* units = MAP_STATE->units;
  int slot = MAP_STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  return AiSeaTransport_CargoReachesEnemyHq();
}
