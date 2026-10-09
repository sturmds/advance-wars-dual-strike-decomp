#include "types.h"
#include "game/unlocks.h"

/* Game progress flags, split over three bit arrays by id range. */
extern u8 data_022a7ef4[]; /* ids 0-31 */
extern u8 data_022a7f50[]; /* ids 32-94 */
extern u8 gGameFlagsHigh[]; /* ids 95-158 */

BOOL GameFlag_IsSet(int id)
{
    if (id >= 95 && id < 159) {
        id -= 95;
        /* needed to match: operands in the other order from the two blocks below */
        return ((1 << (id & 7)) & gGameFlagsHigh[id >> 3]) != 0;
    }
    if (id >= 32 && id < 95) {
        id -= 32;
        return (data_022a7f50[id >> 3] & (1 << (id & 7))) != 0;
    }
    if (id >= 0 && id < 32) {
        return (data_022a7ef4[id >> 3] & (1 << (id & 7))) != 0;
    }
    return FALSE;
}
