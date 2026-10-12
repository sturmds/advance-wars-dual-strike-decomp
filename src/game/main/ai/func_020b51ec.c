#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL AiSeaTransport_CargoReachesEnemyHq(void);
#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_020b51ec(int x, int y) {
  MapUnit* units = STATE->units;
  int slot = STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  return AiSeaTransport_CargoReachesEnemyHq();
}
