#include "types.h"

extern u8 data_ov000_023503a0;
extern u16 data_02166598[];        /* picture number of each CO id */
extern u8 *data_ov000_022f664c[];  /* picture tile data, indexed by data_ov000_023503a0 */

/* The 8 tiles (256 bytes) of the small picture of a CO. */
u8 *func_020ccbe4(int co)
{
    return data_ov000_022f664c[data_ov000_023503a0] + data_02166598[co] * 256;
}
