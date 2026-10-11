/* Needed to match: the original reads income from the player twice and
 * loads data_027c0284 before data_027c027c; mwcc does both only with these
 * optimizations off. */
#pragma opt_common_subs off
#pragma opt_dead_assignments off

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define PLAYER_AT(players, offset) ((Player *)((u32)(players) + (offset)))

int Unit_GetValuePerHp(int playerIndex, int unitType);

/* Used by AiTurn_PickUnitToBuild: TRUE when the build should wait and save
 * funds (income covers the cost within a day or two, and the funds left would
 * still buy two Infantry). */
BOOL AiBuild_ShouldSaveFunds(u32 cost)
{
    int playerIndex;
    u32 offset;
    Player *players;
    u32 income;

    if (data_021619c0->savingTarget != 0) {
        return FALSE;
    }
    {
        int current = data_027c0284;
        Unk_02080440_48 *state = data_027c027c->unk_48[current];
        playerIndex = state->currentPlayer;
        players = state->players;
    }
    offset = playerIndex * sizeof(Player);
    income = PLAYER_AT(players, offset)->unk_08;
    if (cost <= income - data_021619c0->unk_0c) {
        return FALSE;
    }
    if (cost > income * 2 - data_021619c0->unk_0c) {
        return FALSE;
    }
    cost += Unit_GetValuePerHp(playerIndex, UNIT_INFANTRY) * 20;
    return cost < PLAYER_AT(players, offset)->funds + PLAYER_AT(players, offset)->unk_08 - data_021619c0->unk_0c;
}
