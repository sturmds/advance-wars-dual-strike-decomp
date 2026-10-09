#ifndef GAME_UNK_OV005_0236BB80_H
#define GAME_UNK_OV005_0236BB80_H

#include "types.h"

/* Overlay 5 work block (0xc4 bytes) that func_ov005_02352790 allocates
 * into Ov005Task.work and data_ov005_0236bb80. */
typedef struct Unk_ov005_0236bb80 {
    u8 pad_00[0x8];
    u32 unk_08;
    u8 pad_0c[0x78 - 0xc];
    u32 unk_78;
    u8 pad_7c[0xc4 - 0x7c];
} Unk_ov005_0236bb80;

#endif
