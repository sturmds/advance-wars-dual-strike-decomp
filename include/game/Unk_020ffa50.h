#ifndef GAME_UNK_020FFA50_H
#define GAME_UNK_020FFA50_H

#include "types.h"

/* Passed to func_020ffa50 by func_020fd984. unk_7c points to an array of
 * 0x18-byte entries. */
typedef struct Unk_020ffa50_Entry {
    u32 unk_00;
    u8 pad_04[0x18 - 0x04];
} Unk_020ffa50_Entry;

typedef struct Unk_020ffa50 {
    u8 pad_00[0x7c];
    Unk_020ffa50_Entry *unk_7c;
} Unk_020ffa50;

#endif
