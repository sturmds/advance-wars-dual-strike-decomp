#include "types.h"
#include "game/player.h"

int func_020018c4(int player, int slot);   /* CO byte of a player; slot 0 is the front CO, 1 the back CO */

int Player_GetCoId(int player, int slot)
{
    return func_020018c4(player, slot) & PLAYER_CO_ID_MASK;
}
