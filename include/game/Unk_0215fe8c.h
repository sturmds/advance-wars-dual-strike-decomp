#ifndef GAME_UNK_0215FE8C_H
#define GAME_UNK_0215FE8C_H

#include "types.h"

/* Pointed to by data_0215fe8c. Read by link play code (LinkPhase_InitScreen,
 * map select, battle setup) through small getters. */
typedef struct Unk_0215fe8c {
    u8 pad_00[0x0d];
    u8 unk_0d;                         /* a player number, 0-3 (func_0207bd6c) */
    u8 pad_0e[0x1c - 0x0e];
    u32 unk_1c;                        /* 1 in some link mode (func_0207bd80) */
} Unk_0215fe8c;

#endif
