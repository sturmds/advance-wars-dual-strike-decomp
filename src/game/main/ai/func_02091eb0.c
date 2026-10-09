#include "types.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_0229e44c[][4];
extern s8 data_0229e5c0[][12];

#define STATE (data_027c027c->unk_48[data_027c0284])

int Unit_GetValuePerHp(int playerIndex, int unitType);
int func_0209622c(void);
void func_0209316c(int index);

/* End-actions step 1. Like AiTurn_TryBuildFighter, for unit type 14
 * (UNIT_ANTI_AIR), but gated on the sea bit of domainsOwn, which does not fit
 * that type, and it also clears bit 1 of byte 3 in the record that
 * func_0209622c's pick refers to. Both are unexplained, so the name stays.
 * Returns 1 when it queued an action. */
BOOL func_02091eb0(void)
{
    int index;
    u8 *record;

    if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
        return 0;
    }
    if (gAiProfile->units[UNIT_ANTI_AIR - 1].unk_00 == 0) {
        return 0;
    }
    if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_ANTI_AIR) * 10) > STATE->players[STATE->currentPlayer].funds) {
        return 0;
    }
    index = func_0209622c();
    if (index == -1) {
        return 0;
    }
    record = data_0229e44c[data_0229e5c0[index][0]];
    record[3] &= ~2;
    gUnitAction.target = UNIT_ANTI_AIR;
    func_0209316c(index);
    return 1;
}
