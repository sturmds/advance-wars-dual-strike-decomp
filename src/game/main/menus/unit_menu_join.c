#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void Unit_SubtractMoveFuel(MapUnit* unit, int tileCost);
void ActingUnit_SaveStartPos(void);
void UnitAction_Join(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Unit command menu: Join. When gLinkPlay is set, takes the move's fuel off the
 * acting unit first (not in fog or rain) and sends unit action 10. */
void UnitMenu_Join(void) {
  func_020b9d48();
  func_0201e954();
  if (gLinkPlay != 0) {
    if (MAP_STATE->unk_1c4->fog == 0 &&
        MAP_STATE->unk_1c4->weather != WEATHER_RAIN) {
      Unit_SubtractMoveFuel(MAP_STATE->actingUnit, MAP_STATE->unk_46);
      MAP_STATE->unk_46 = 0;
    }
  }
  if (gLinkPlay != 0) {
    UnitAction_Send(10, MAP_STATE->actingUnitIndex, 0, 0);
  }
  ActingUnit_SaveStartPos();
  UnitAction_Join();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
}
