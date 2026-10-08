#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int BadLuckDamage(int playerIndex, int coSlot);

/* Bad luck roll maximum of a player: the front CO's value for its current
 * power state plus skills. 0 when unk_5d is clear. */
int func_020dff88(int playerIndex)
{
    int badLuck = 0;
    const PlayerCo *co;

    if (data_027c027c->unk_48[data_027c0284]->unk_5d == 0) {
        return badLuck;
    }
    badLuck += BadLuckDamage(playerIndex, 0);
    /* 97.8%: retyping Player.frontCo as PlayerCo and reading it without
     * this cast gives a full match (register order only differs). */
    co = (const PlayerCo *)&data_027c027c->unk_48[data_027c0284]->players[playerIndex].frontCo;
    return badLuck + gCoTable[co->id & PLAYER_CO_ID_MASK].blocks[co->powerState].badLuck;
}
