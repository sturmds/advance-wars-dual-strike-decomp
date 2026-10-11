#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
int Map_IsThisMapsTurn(void);
void func_02080e84(void);
int func_02080fd8(u8 *dest);
void func_020808a4(u8 a, u8 b);

/* Sends this player's reaction buttons when it is not their turn, then shows
 * the (player, button) pairs received from the other side. */
void LinkReaction_Update(void)
{
    u8 pair[2]; /* link slot, button */

    if (data_027c027c->unk_48[data_027c0284]->unk_5b == 0) {
        int isLocalTurn; /* needed to match: the original keeps the test as a 0/1 value */

        if (data_027c027c->unk_48[data_027c0284]->currentPlayer == Link_FindSlotOfPlayer(func_0207bd6c())
            && Map_IsThisMapsTurn() == 1) {
            isLocalTurn = 1;
        } else {
            isLocalTurn = 0;
        }
        if (!isLocalTurn) {
            func_02080e84();
        }
    }
    if (func_02080fd8(pair) != 1) {
        return;
    }
    do {
        func_020808a4(pair[0], pair[1]);
    } while (func_02080fd8(pair) == 1);
}
