#include "types.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

int Unit_GetValuePerHp(int playerIndex, int unitType);
int func_02093b50(void);
int func_02093bfc(void);
void func_0209a91c(void);
int func_02095e0c(void);
void func_0209316c(int index);

/* End-actions step 3. Tries to start building a Fighter: needs an airport among
 * the current player's factories, the profile's flag for Fighters set, the
 * funds for one and a yes from func_02093b50 and func_02093bfc (not decompiled;
 * the first must not return 1, the second must be nonzero). func_02095e0c
 * picks the factory (-1: none). The name is a guess from the unit type and the
 * checks. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildFighter(void)
{
    int index;

    if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_AIR)) {
        return 0;
    }
    if (gAiProfile->units[UNIT_FIGHTER - 1].unk_00 == 0) {
        return 0;
    }
    if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_FIGHTER) * 10) > STATE->players[STATE->currentPlayer].funds) {
        return 0;
    }
    if (func_02093b50() == 1) {
        return 0;
    }
    if (func_02093bfc() == 0) {
        return 0;
    }
    func_0209a91c();
    index = func_02095e0c();
    if (index == -1) {
        return 0;
    }
    gUnitAction.target = UNIT_FIGHTER;
    func_0209316c(index);
    return 1;
}
