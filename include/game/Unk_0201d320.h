#ifndef GAME_UNK_0201D320_H
#define GAME_UNK_0201D320_H

#include "types.h"

/* 16-byte record; the handlers step through an array of these. */
typedef struct Unk_0201d320_Entry {
    u8 pad_00[0xc];
    u32 unk_0c;
} Unk_0201d320_Entry;

/* Element of data_022977f4 (24 bytes each). */
typedef struct Unk_0201d320 {
    Unk_0201d320_Entry *unk_00; /* current record */
    u8 pad_04[0x4];
    u16 unk_08;
    u8 pad_0a[0xe];
} Unk_0201d320;

#endif
