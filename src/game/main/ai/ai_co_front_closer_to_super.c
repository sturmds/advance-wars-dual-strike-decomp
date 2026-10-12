#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int SuperPowerCost(int player, int slot);
int AiCo_TagRank(int coId);   /* a rank per CO id, used to break ties */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Needed to match (the same trick as AiUnit_HasFuelForTile): at these points
 * the original reads data_027c0284 and then data_027c027c again instead of
 * reusing a state pointer it holds. Volatile reads, index first, are the only
 * form found that does this. */
#define RELOAD_STATE(dst)                                                    \
    {                                                                        \
        int index_ = *(volatile int *)&data_027c0284;                        \
        dst = (*(Unk_02080440 *volatile *)&data_027c027c)->unk_48[index_];   \
    }

/* Run by AiTurn_RunPowerStep while no tag power is on; TRUE makes it perform
 * AI action 15. Needs CO effects on, no power on and a back CO. Compares how
 * far each CO's meter is from its Super CO Power cost: TRUE when the front CO
 * is closer. When both are equally far, or both meters are empty,
 * AiCo_TagRank ranks the two COs instead. */
BOOL AiCo_FrontCloserToSuper(void)
{
    int checkedPlayer;              /* the current player, read during the checks */
    u32 hasPartner;
    Unk_02080440_48 *state;
    int player;                     /* the current player again */
    Player *players;
    u32 frontCharge;
    int current;

    /* Needed to match: the first index read goes through frontCharge, which
     * gets its real value later (7 differing instructions without it, 4 with) */
    frontCharge = *(volatile int *)&data_027c0284;
    state = data_027c027c->unk_48[frontCharge];
    if (state->coEffectsOn == 0) {
        return FALSE;
    }
    checkedPlayer = state->currentPlayer;
    if (state->players[checkedPlayer].front.powerState != 0) {
        return FALSE;
    }
    hasPartner = state->players[checkedPlayer].back.id != 0;
    if (hasPartner == FALSE) {
        return FALSE;
    }
    RELOAD_STATE(state);
    player = state->currentPlayer;
    /* Needed to match (found by tools/permuter.py): this empty test emits
     * no code, but naming players[player] here, before players is set,
     * gives the players base and the index product the original's registers */
    if (players[player].back.charge || players[player].back.charge) {
    }
    frontCharge = state->players[player].front.charge;
    players = state->players;
    /* needed to match: the byte-offset form keeps the compiler from reusing
     * the index product of players[player] */
    if (SuperPowerCost(player, 0) - frontCharge
            == SuperPowerCost(checkedPlayer, 1)
                   - ((Player *)((u8 *)state->players + player * sizeof(Player)))->back.charge
        || (frontCharge == 0 && players[player].back.charge == 0)) {
        if (AiCo_TagRank(STATE->players[STATE->currentPlayer].front.id & PLAYER_CO_ID_MASK)
            < AiCo_TagRank(players[player].back.id & PLAYER_CO_ID_MASK)) {
            return TRUE;
        }
    } else {
        RELOAD_STATE(state);
        current = state->currentPlayer;
        if (SuperPowerCost(current, 0) - state->players[state->currentPlayer].front.charge
            < SuperPowerCost(player, 1) - state->players[current].back.charge) {
            return TRUE;
        }
    }
    return FALSE;
}
