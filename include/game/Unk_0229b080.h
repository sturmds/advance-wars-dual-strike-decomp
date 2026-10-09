#ifndef GAME_UNK_0229B080_H
#define GAME_UNK_0229B080_H

#include "types.h"

/* One of 16 0x30-byte slots in data_0229b080. A slot is free when its first
 * word is 0; func_02021c84 finds a slot by that word. */
typedef struct Unk_0229b080 {
    u32 id;
    u8 pad_04[0x30 - 0x04];
} Unk_0229b080;

#define UNK_0229B080_COUNT 16

extern Unk_0229b080 data_0229b080[UNK_0229B080_COUNT];

#endif
