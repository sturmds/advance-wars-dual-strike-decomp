#include "types.h"

extern u8 data_0214c79c[];

u8 Player_GetArmyStyle(int player);

/* Returns the 256-byte block of the player's army style; odd and even players use the style's two blocks. */
u8 *func_020dbde8(int player)
{
    int style = Player_GetArmyStyle(player);

    return data_0214c79c + (((player - 1) & 1) + (style - 1) * 2) * 256;
}
