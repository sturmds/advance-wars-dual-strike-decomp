#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Player_GetFuelRate(int player);
int func_020e0360(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Daily fuel use added on top of func_020e0360's value, per turn spent
 * travelling. */
#define FUEL_PER_DAY_STEALTH 8
#define FUEL_PER_DAY_AIR_SUB 5
#define FUEL_PER_DAY_OTHER   1

/* TRUE when the acting unit can reach (x, y) with fuel left over for the
 * extra days. rangeMap holds the move cost there; the days are the whole
 * turns beyond the first at movesPerTurn. Ground units and unreached tiles
 * (cost 0) always pass. */
BOOL func_020a6554(int x, int y, int movesPerTurn)
{
    int days;
    int unitType;
    int fuelRate;
    Unk_02080440_48 *state;

    if (!(Ov000_gUnitStats[STATE->actingUnit->type].moveGroup & (MOVE_GROUP_AIR | MOVE_GROUP_SEA))) {
        return TRUE;
    }
    if (STATE->rangeMap[y][x] == 0) {
        return TRUE;
    }
    days = HwDiv32(STATE->rangeMap[y][x] - 1, movesPerTurn);
    state = STATE;
    unitType = state->actingUnit->type;
    if (unitType == UNIT_STEALTH) {
        fuelRate = Player_GetFuelRate(state->currentPlayer);
        if (state->actingUnit->fuel - fuelRate * state->rangeMap[y][x] < days * (func_020e0360(state->currentPlayer, unitType) + FUEL_PER_DAY_STEALTH)) {
            return FALSE;
        }
    } else if ((Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) || unitType == UNIT_SUB) {
        fuelRate = Player_GetFuelRate(STATE->currentPlayer);
        if (STATE->actingUnit->fuel - fuelRate * STATE->rangeMap[y][x] < days * (func_020e0360(STATE->currentPlayer, unitType) + FUEL_PER_DAY_AIR_SUB)) {
            return FALSE;
        }
    } else {
        fuelRate = Player_GetFuelRate(state->currentPlayer);
        if (state->actingUnit->fuel - fuelRate * state->rangeMap[y][x] < days * (func_020e0360(state->currentPlayer, unitType) + FUEL_PER_DAY_OTHER)) {
            return FALSE;
        }
    }
    return TRUE;
}
