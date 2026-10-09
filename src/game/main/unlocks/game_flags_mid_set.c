#include "types.h"
#include "game/unlocks.h"

extern u8 data_022a7f50[];

/* Sets flag `id` of game progress flags 32-94 (id counted from 32) to value (0 or 1). */
void GameFlagsMid_Set(u32 id, u8 value)
{
    int bit = 1 << (id & 7);
    data_022a7f50[id >> 3] = (data_022a7f50[id >> 3] & ~bit) | (bit * value);
}
