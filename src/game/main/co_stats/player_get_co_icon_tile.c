#include "types.h"

extern u16 data_ov000_022f5d88[][27];  /* per army style, then per CO id: icon tile / 4 */

u8 Player_GetArmyStyle(int playerIndex);

/* First tile of the CO icon in the tile bank, from the army style of player. */
int Player_GetCoIconTile(int player, int coId)
{
    return data_ov000_022f5d88[Player_GetArmyStyle(player) - 1][coId] << 2;
}
