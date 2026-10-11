#include "types.h"

extern u16 data_ov000_022f5d88[][27];  /* per army style, then per unit type: icon tile / 4 */

u8 Player_GetArmyStyle(int playerIndex);

/* First tile of a unit type's icon in the tile bank, in the army style of
 * player. */
int Player_GetUnitIconTile(int player, int unitType)
{
    return data_ov000_022f5d88[Player_GetArmyStyle(player) - 1][unitType] << 2;
}
