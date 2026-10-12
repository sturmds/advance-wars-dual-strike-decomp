#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActingUnit_ResetCaptureIfMoved(void);
void func_02017cf4(void);
void Unit_SubtractMoveFuel(
    MapUnit* unit,
    int tileCost); /* takes the fuel used for the move off the unit */
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020c7398(int unitSlot); /* removes the unit */
void func_ov000_022af35c(MapUnit* unit);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Ends the acting unit's action: puts it on its destination tile, takes off
 * the fuel used (not in fog or rain), marks it waited and removes it when
 * its HP are 0. */
void ActingUnit_FinishAction(void) {
  ActingUnit_ResetCaptureIfMoved();
  func_02017cf4();
  GAME->actingUnit->x = GAME->actingUnitDestX;
  GAME->actingUnit->y = GAME->actingUnitDestY;
  if (GAME->unk_1c4->fog == 0 && GAME->unk_1c4->weather != WEATHER_RAIN) {
    Unit_SubtractMoveFuel(GAME->actingUnit, GAME->unk_46);
  }
  UnitShowTask_DestroyRunning();
  GAME->actingUnit->flags |= MAP_UNIT_WAITED;
  if (GAME->actingUnit->flags & MAP_UNIT_CARRIED) {
    GAME->actingUnit->flags |= MAP_UNIT_SELECTED;
  }
  func_020c8888();
  if (GAME->actingUnit->hp == 0) {
    func_020c7398(GAME->actingUnit - GAME->units);
  }
  func_ov000_022af35c(GAME->actingUnit);
}
