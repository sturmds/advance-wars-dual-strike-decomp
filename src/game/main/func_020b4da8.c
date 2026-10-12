#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void AiSeaTransport_CargoReachesHq(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills the range map from (x, y) for the unit that the acting unit
 * carries in its second cargo slot, then runs AiSeaTransport_CargoReachesHq. */
void func_020b4da8(int x, int y) {
  MapUnit* units = STATE->units;
  int slot = STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  AiSeaTransport_CargoReachesHq();
}
