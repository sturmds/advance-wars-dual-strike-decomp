#include "types.h"
#include "game/unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

extern u32 gRngState;
extern u8 data_0229ea0c[];
extern u32 data_0229eaec;

void AiUnit_AdjustAction(int x, int y, u8 *action, u8 *target, u8 *arg);
void func_020a3ab4(int action);
void func_021296a8(void *p, int n);
void func_01ffa44c(MapUnit *unit);
void func_01ffa02c(int x, int y, u8 *out);
void PackNibblePairs(const u8 *nibbles, u8 *out);
void func_020b81b4(int x, int y, int action, int target, int arg);
int func_020c8330(MapUnit *unit, int n);
void func_0208fda8(void);

/* Makes the active unit move to (x, y) and do action there, and records
 * the action in gUnitAction so the turn can show it. AiUnit_AdjustAction
 * may change action, target and arg first. */
void AiUnit_PerformAction(int x, int y, u8 action, u8 target, u8 arg)
{
    MapUnit *unit;
    int fuelUsed;

    AiUnit_AdjustAction(x, y, &action, &target, &arg);
    func_020a3ab4(action);
    unit = GAME->actingUnit;
    if (x == unit->x && y == unit->y && action == 2) {
        unit->unk_08_7 = 1;
        func_021296a8(data_0229ea0c, 1);
    }
    func_01ffa44c(unit);
    func_01ffa02c(x, y, GAME->unk_170);
    PackNibblePairs(GAME->unk_170, gUnitAction.packed);
    func_020b81b4(x, y, action, target, arg);

    gUnitAction.unitIndex = GAME->actingUnitIndex;
    gUnitAction.rngState = gRngState;
    gUnitAction.fromX = GAME->actingUnitStartX;
    gUnitAction.fromY = GAME->actingUnitStartY;
    gUnitAction.x = x;
    gUnitAction.y = y;
    gUnitAction.action = action;
    gUnitAction.target = target;
    gUnitAction.unk_07 = arg;
    fuelUsed = func_020c8330(GAME->actingUnit, GAME->unk_46);
    gUnitAction.fuelLeft = GAME->actingUnit->fuel - fuelUsed;
    gUnitAction.pending = TRUE;
    if (data_0229eaec != 0) {
        func_0208fda8();
    }
    func_021296a8(data_0229ea0c, 1);
}
