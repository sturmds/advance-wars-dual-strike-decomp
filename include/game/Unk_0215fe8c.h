#ifndef GAME_UNK_0215FE8C_H
#define GAME_UNK_0215FE8C_H

#include "types.h"

/* Pointed to by data_0215fe8c. Read by link play code (LinkPhase_InitScreen,
 * map select, battle setup) through small getters. */
typedef struct Unk_0215fe8c {
    u8 pad_00[0x08];
    u16 unk_08;                        /* func_0207a1dc result, stored when func_0207bb48 stops the task */
    u8 pad_0a[2];
    u8 unk_0c;                         /* func_0207bb48: 0 while the data_0215feb8 task runs, else 1 */
    u8 unk_0d;                         /* a player number, 0-3 (func_0207bd6c) */
    u8 pad_0e[0x1c - 0x0e];
    u32 unk_1c;                        /* 1 in some link mode (func_0207bd80) */
} Unk_0215fe8c;

#endif
