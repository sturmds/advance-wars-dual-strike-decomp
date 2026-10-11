#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

#define PLAYER_AT(players, offset) ((Player *)((u32)(players) + (offset)))

int Unit_GetValuePerHp(int playerIndex, int unitType);

/* WIP, 75%: the original keeps the player array base and offset in separate
 * registers and does not reuse the player pointer after the call.
 * Used by AiTurn_PickUnitToBuild: TRUE when the build should wait and save
 * funds (income covers the cost within a day or two, and the funds left would
 * still buy two Infantry). */
BOOL func_02098238(u32 cost)
{
    int playerIndex;
    Player *players;
    u32 offset;
    u32 income;

    if (data_021619c0->savingTarget != 0) {
        return FALSE;
    }
    playerIndex = STATE->currentPlayer;
    offset = playerIndex * sizeof(Player);
    players = STATE->players;
    income = PLAYER_AT(players, offset)->unk_08;
    if (cost <= income - data_021619c0->unk_0c) {
        return FALSE;
    }
    if (cost > income * 2 - data_021619c0->unk_0c) {
        return FALSE;
    }
    cost += Unit_GetValuePerHp(playerIndex, UNIT_INFANTRY) * 20;
    return cost < *(s32 *)((u8 *)players + offset) + PLAYER_AT(players, offset)->unk_08 - data_021619c0->unk_0c;
}
