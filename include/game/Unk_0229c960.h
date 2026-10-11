#ifndef GAME_UNK_0229C960_H
#define GAME_UNK_0229C960_H

#include "types.h"

/* data_0229c960 (main bss). */
typedef struct Unk_0229c960 {
    u8 pad_00[0x08];
    u32 ggid;                          /* compared with a scanned parent's game group id */
    u8 pad_0c[0x10 - 0x0c];
    u16 unk_10;
} Unk_0229c960;

extern Unk_0229c960 data_0229c960;

#endif
