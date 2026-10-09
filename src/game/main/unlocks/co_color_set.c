#include "types.h"
#include "game/unlocks.h"

extern u8 data_022a7f80[]; /* one bit per CO: alternate colour unlocked */

void CoColor_Set(u32 coId, int value)
{
    int bit = 1 << (coId & 7);
    data_022a7f80[coId >> 3] = (~bit & data_022a7f80[coId >> 3]) | (bit * value);
}
