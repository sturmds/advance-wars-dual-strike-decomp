#ifndef GAME_UNK_020D8C8C_H
#define GAME_UNK_020D8C8C_H

#include "types.h"

/* Object that holds a handle at offset 0x50, released by SpriteAnim_Free. */
typedef struct Unk_020d8c8c {
    u8 pad_00[0x50];
    u32 *handle;
} Unk_020d8c8c;

#endif
