#include "types.h"

extern u16 data_ov000_022f5d88[][27];  /* per army style, then per icon: tile / 4 */

/* First tile of icon number index in the set of an army style (1 to 5). */
int ArmyStyle_GetIconTile(int armyStyle, int index)
{
    return data_ov000_022f5d88[armyStyle - 1][index] << 2;
}
