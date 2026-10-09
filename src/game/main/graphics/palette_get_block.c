#include "types.h"

extern u8 data_0214a79c[];

u8 *func_020dbde8(int player);

/* Returns the 256-byte palette block `index` of the table. Index 8 depends on
 * the player: func_020dbde8 picks the block from the player's army style. */
u8 *Palette_GetBlock(int index, int player)
{
    if (index != 8) {
        return data_0214a79c + index * 256;
    }
    return func_020dbde8(player);
}
