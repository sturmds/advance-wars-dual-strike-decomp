#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_02153648[][136];

u32 Rng_Next(void);
void CoQuote_StartText(u32 script);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Starts one of the six random lines of the player's front CO (a row of
 * data_02153648 per CO). CO ids with bit 7 set get a fixed line. Returns 0. */
int CoQuote_StartRandom(int player)
{
    if (GAME->players[player].front.id & 0x80) {
        CoQuote_StartText(0xf000028a);
    } else {
        CoQuote_StartText(data_02153648[GAME->players[player].front.id & PLAYER_CO_ID_MASK][Rng_Next() % 6]);
    }
    return 0;
}
