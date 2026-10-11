#include "types.h"
#include "game/Unk_02080440.h"
#include "game/link_reaction_queue.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
int Map_IsThisMapsTurn(void);
void func_02080e84(void);
int LinkReactionQueue_Pop(LinkReaction *dest);
void LinkReaction_Show(u8 a, u8 b);

/* Sends this player's reaction buttons when it is not their turn, then shows
 * the (player, button) pairs received from the other side. */
void LinkReaction_Update(void)
{
    LinkReaction reaction;

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
    if (LinkReactionQueue_Pop(&reaction) != 1) {
        return;
    }
    do {
        LinkReaction_Show(reaction.slot, reaction.button);
    } while (LinkReactionQueue_Pop(&reaction) == 1);
}
