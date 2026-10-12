#include "types.h"

extern u8
    data_022a7f58[]; /* 11 flags, all set by the unlock-everything routine */

u8 func_02101964(u32 id) { return (1 << (id & 7)) & data_022a7f58[id >> 3]; }
