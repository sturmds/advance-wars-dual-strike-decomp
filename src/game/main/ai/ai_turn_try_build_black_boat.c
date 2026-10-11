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
int func_020961ac(int arg);
int func_02090ed4(void);
void func_0209316c(int index);

/* End-actions step 8, fourth sub-step. Tries to start building a Black
 * Boat: needs bit 2 of the profile byte that func_020985b0 tests and the
 * funds for one. func_020961ac(3) picks the factory (-1: none).
 * func_02090ed4 counts the player's ships that have a weapon; the AI wants
 * one Black Boat per three of them, the new one included (a ship count of
 * 2 is enough for the first). The name is a guess from the unit type.
 * Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildBlackBoat(void)
{
    Unk_02080440_48 *state;
    int index;
    int boats;

    if (func_020985b0(4) == 0) {
        return 0;
    }
    state = STATE;
    if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_BLACK_BOAT) * 10) > state->players[state->currentPlayer].funds) {
        return 0;
    }
    index = func_020961ac(3);
    if (index == -1) {
        return 0;
    }
    boats = Ai_CountUnitsOfType(UNIT_BLACK_BOAT);
    if (func_02090ed4() + 1 < (boats + 1) * 3) {
        return 0;
    }
    gUnitAction.target = UNIT_BLACK_BOAT;
    func_0209316c(index);
    return 1;
}
