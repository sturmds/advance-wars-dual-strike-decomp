#ifndef GAME_UNK_02080440_H
#define GAME_UNK_02080440_H

#include "types.h"

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x2a];
    u16 unk_2a;
} Unk_02080440_1c4;

typedef struct Unk_02080440_48 {
    u8 pad_00[0x1c4];
    Unk_02080440_1c4 *unk_1c4;
} Unk_02080440_48;

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x48];
    Unk_02080440_48 *unk_48[1]; /* length unknown, indexed by data_027c0284 */
} Unk_02080440;

#endif
