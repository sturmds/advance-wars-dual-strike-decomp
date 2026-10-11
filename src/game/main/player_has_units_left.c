#include "types.h"

BOOL Player_IsInGame(u8 player);
int Player_CountUnitsBuilt(int player);
BOOL func_020c0764(int player);

/* Whether the player is still in the battle by unit count: FALSE when out
 * of the game, TRUE when no unit has been built yet, else func_020c0764
 * (a unit in slots 1 to 50). */
BOOL Player_HasUnitsLeft(int player)
{
    if (!Player_IsInGame(player)) {
        return FALSE;
    }
    if (Player_CountUnitsBuilt(player) == 0) {
        return TRUE;
    }
    return func_020c0764(player);
}
