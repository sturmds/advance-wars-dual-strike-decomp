#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* CoRecord.theme of CO 0; the code addresses the field through this label. */
extern u16 data_02153620;

/* Theme song id of a player's front CO. */
int Player_GetCoThemeSong(int playerIndex)
{
    int co = data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id & PLAYER_CO_ID_MASK;

    return *(u16 *)((u8 *)&data_02153620 + co * sizeof(CoRecord));
}
