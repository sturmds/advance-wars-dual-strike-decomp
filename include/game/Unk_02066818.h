#ifndef GAME_UNK_02066818_H
#define GAME_UNK_02066818_H

#include "types.h"

typedef struct Unk_02066818 {
    u8 pad_000[0x2c8];
    u8 unk_2c8[0x1b]; /* team of each player slot, 0-3 */
    u8 unk_2e3;       /* number of player slots */
} Unk_02066818;

#endif
