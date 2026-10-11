#include "types.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE (*(Unk_02080440_48 **)((u8 *)(data_027c0284 * sizeof(Unk_02080440_48 *)) + (u32)data_027c027c \
                                     + (u32)&((Unk_02080440 *)0)->unk_48))

int Ai_CountUnitsOfType(int type);
int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL func_020985b0(int mask);
int func_02090360(void);
int func_02096838(int arg);
int AiSite_PickFirst(int arg);
int func_02090fa8(void);
int func_02091068(void);
int func_02091128(int type);
void func_0209316c(int index);

/* End-actions step 8, second sub-step. Tries to start building a Lander:
 * needs bit 3 of the profile byte that func_020985b0 tests and the funds for
 * one. func_02090360 runs first and ends the step when it returns 1.
 * func_02096838(3), then AiSite_PickFirst(3), pick the factory (-1: none).
 * func_02090fa8 counts the player's vehicles (type above Mech) with
 * MapUnit.transportWanted 2, func_02091068 the foot units with it, and
 * func_02091128 the player's units of a type with nothing in cargo[0].
 * With no vehicle waiting nothing is built. When Landers exist already, the
 * new one needs four waiting units per empty Lander, itself included,
 * counting foot units only past four per empty Black Boat. The name is a
 * guess from the unit type. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildLander(void)
{
    Unk_02080440_48 *state;
    int vehiclesWaiting;
    int footWaiting;
    int index;
    int emptyLandersAfter;
    int count;

    if (func_020985b0(8) == 0) {
        return 0;
    }
    state = STATE;
    if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_LANDER) * 10) > state->players[state->currentPlayer].funds) {
        return 0;
    }
    if (func_02090360() == 1) {
        return 1;
    }
    index = func_02096838(3);
    if (index == -1) {
        index = AiSite_PickFirst(3);
        if (index == -1) {
            return 0;
        }
    }
    count = Ai_CountUnitsOfType(UNIT_LANDER);
    vehiclesWaiting = func_02090fa8();
    if (vehiclesWaiting == 0) {
        return 0;
    }
    if (count != 0) {
        emptyLandersAfter = func_02091128(UNIT_LANDER) + 1;
        count = func_02091128(UNIT_BLACK_BOAT);
        footWaiting = func_02091068() - count * 4;
        if (footWaiting < 0) {
            footWaiting = 0;
        }
        if (emptyLandersAfter * 4 > vehiclesWaiting + footWaiting) {
            return 0;
        }
    }
    gUnitAction.target = UNIT_LANDER;
    func_0209316c(index);
    return 1;
}
