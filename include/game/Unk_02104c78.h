#ifndef GAME_UNK_02104C78_H
#define GAME_UNK_02104C78_H

#include "types.h"

typedef struct Unk_02104c78_24 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} Unk_02104c78_24;

/* Object passed to func_02104b68 and func_02104c78 (main). */
typedef struct Unk_02104c78 {
    u8 pad_00[0x18];
    u32 unk_18;
    u8 pad_1c[0x24 - 0x1c];
    Unk_02104c78_24 unk_24;
} Unk_02104c78;

#endif
