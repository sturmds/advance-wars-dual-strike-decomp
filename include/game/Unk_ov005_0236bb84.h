#ifndef GAME_UNK_OV005_0236BB84_H
#define GAME_UNK_OV005_0236BB84_H

#include "types.h"

/* Overlay 5 work block (0x24 bytes) that func_ov005_023551f8 allocates
 * into Ov005Task.work and data_ov005_0236bb84. */
typedef struct Unk_ov005_0236bb84 {
    u8 pad_00[0x8];
    int unk_08;
    u8 pad_0c[0x24 - 0x0c];
} Unk_ov005_0236bb84;

#endif
