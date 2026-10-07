#ifndef GAME_UNK_02025BFC_H
#define GAME_UNK_02025BFC_H

#include "types.h"

/* One entry of the 80-entry table at data_0229b3b0. */
typedef struct Unk_02025bfc {
    u8 unk_00;
    u8 unk_01;
    u8 pad_02[0x1a];
    u16 unk_1c; /* flag bits, read by func_02026404 */
    u8 pad_1e[0x1e];
} Unk_02025bfc;

#endif
