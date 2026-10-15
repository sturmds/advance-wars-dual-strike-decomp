#include "types.h"
#include "game/Unk_02080440.h"

void ActingUnit_ResetCaptureIfMoved(void);
void func_02017cf4(void);
void Unit_SubtractMoveFuel(
    MapUnit* unit,
    int tileCost); /* takes the fuel used for the move off the unit */
void UnitShowTask_DestroyRunning(void);
void func_020c8888(void);
void func_020c7398(int unitSlot); /* removes the unit */
void MapScript_RunList0c(MapUnit* unit);

/* Ends the acting unit's action: puts it on its destination tile, takes off
 * the fuel used (not in fog or rain), marks it waited and removes it when
 * its HP are 0. */
void ActingUnit_FinishAction(void) {
  ActingUnit_ResetCaptureIfMoved();
  func_02017cf4();
  MAP_STATE->actingUnit->x = MAP_STATE->actingUnitDestX;
  MAP_STATE->actingUnit->y = MAP_STATE->actingUnitDestY;
  if (MAP_STATE->unk_1c4->fog == 0 &&
      MAP_STATE->unk_1c4->weather != WEATHER_RAIN) {
    Unit_SubtractMoveFuel(MAP_STATE->actingUnit, MAP_STATE->unk_46);
  }
  UnitShowTask_DestroyRunning();
  MAP_STATE->actingUnit->flags |= MAP_UNIT_WAITED;
  if (MAP_STATE->actingUnit->flags & MAP_UNIT_CARRIED) {
    MAP_STATE->actingUnit->flags |= MAP_UNIT_SELECTED;
  }
  func_020c8888();
  if (MAP_STATE->actingUnit->hp == 0) {
    func_020c7398(MAP_STATE->actingUnit - MAP_STATE->units);
  }
  MapScript_RunList0c(MAP_STATE->actingUnit);
}
