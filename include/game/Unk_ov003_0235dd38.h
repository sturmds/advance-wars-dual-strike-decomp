#ifndef GAME_UNK_OV003_0235DD38_H
#define GAME_UNK_OV003_0235DD38_H

#include "types.h"

/* Overlay 3 object passed to func_ov003_0235dd38 and func_ov003_02363ba4,
 * which both clear its halfword at 0x4c. */
typedef struct Unk_ov003_0235dd38 {
    u8 pad_00[0x4c];
    u16 unk_4c;
    u8 pad_4e[0x64 - 0x4e];
    u16 unk_64;
} Unk_ov003_0235dd38;

#endif
