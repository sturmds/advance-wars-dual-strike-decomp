#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_0229eaec;

void func_020b9d48(void);
void func_0201e954(void);
void Unit_SubtractMoveFuel(MapUnit *unit, int tileCost);
void ActingUnit_SaveStartPos(void);
void UnitAction_Join(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit command menu: Join. When data_0229eaec is set, takes the move's fuel off the
 * acting unit first (not in fog or rain) and sends unit action 10. */
void UnitMenu_Join(void)
{
    func_020b9d48();
    func_0201e954();
    if (data_0229eaec != 0) {
        if (GAME->unk_1c4->fog == 0 && GAME->unk_1c4->weather != WEATHER_RAIN) {
            Unit_SubtractMoveFuel(GAME->actingUnit, GAME->unk_46);
            GAME->unk_46 = 0;
        }
    }
    if (data_0229eaec != 0) {
        UnitAction_Send(10, GAME->actingUnitIndex, 0, 0);
    }
    ActingUnit_SaveStartPos();
    UnitAction_Join();
    ActingUnit_FinishAction();
    ActingUnitStart_Restore();
}
