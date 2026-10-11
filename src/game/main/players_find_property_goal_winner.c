#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* unk_81 (from the map header) is a property count goal (guess: it ends the battle); 0: no
 * such goal. Returns the first player slot that owns that many, or 0. */
int Players_FindPropertyGoalWinner(void)
{
    u8 player;

    if (STATE->unk_81 == 0) {
        return 0;
    }
    for (player = 1; player < 5; player++) {
        if (STATE->players[player].unk_15 >= STATE->unk_81) {
            return player;
        }
    }
    return 0;
}
