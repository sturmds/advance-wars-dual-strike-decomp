#ifndef GAME_UNK_02080440_H
#define GAME_UNK_02080440_H

#include "types.h"

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x2a];
    u16 unk_2a;
} Unk_02080440_1c4;

typedef struct Unk_02080440_48 {
    u8 pad_00[0x3e];
    s16 unk_3e;
    s16 unk_40;
    s16 unk_42;
    s16 unk_44;
    u8 pad_46[0x170 - 0x46];
    u8 unk_170[12]; /* 12 values of 0-15, see PackNibblePairs */
    u8 pad_17c[0x1c4 - 0x17c];
    Unk_02080440_1c4 *unk_1c4;
} Unk_02080440_48;

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x48];
    Unk_02080440_48 *unk_48[1]; /* length unknown, indexed by data_027c0284 */
} Unk_02080440;

#endif
