#ifndef GAME_UNK_0229C960_H
#define GAME_UNK_0229C960_H

#include "types.h"

/* data_0229c960 (main bss). */
typedef struct Unk_0229c960 {
    u8 pad_00[0x08];
    u32 ggid;                          /* compared with a scanned parent's game group id */
    u16 tgid;                          /* temporary group id, changes for every parent start */
    u8 pad_0e[0x10 - 0x0e];
    u16 unk_10;
    u8 pad_12[0x32 - 0x12];
    u16 channel;
    u16 parentMaxSize;                 /* largest packet a parent sends */
    u16 childMaxSize;                  /* largest packet a child sends */
    u8 pad_38[0x40 - 0x38];
} Unk_0229c960;

extern Unk_0229c960 data_0229c960;

#endif
