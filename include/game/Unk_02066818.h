#ifndef GAME_UNK_02066818_H
#define GAME_UNK_02066818_H

#include "types.h"

typedef struct Unk_02066818 {
    u8 unk_000[0x20];
    u8 unk_020[4];
    u8 unk_024[4];    /* army colour of each player, from the map header */
    u8 unk_028[4];
    u8 unk_02c;       /* number of players on the map */
    u8 pad_02d[0x2c8 - 0x2d];
    u8 unk_2c8[0x1b]; /* team of each player slot, 0-3 */
    u8 unk_2e3;       /* number of player slots */
} Unk_02066818;

#endif
