#ifndef GAME_UNK_022A7FAC_H
#define GAME_UNK_022A7FAC_H

#include "types.h"

/* Main bss state at data_022a7fac: twelve signed bytes, cleared together
 * by func_021023f4 and read one by one by small getters. */
typedef struct Unk_022a7fac {
    u8 pad_00[4];
    s8 unk_04;
    u8 pad_05[7];
} Unk_022a7fac;

#endif
