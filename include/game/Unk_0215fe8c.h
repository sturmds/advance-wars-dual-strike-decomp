#ifndef GAME_UNK_0215FE8C_H
#define GAME_UNK_0215FE8C_H

#include "types.h"

/* Pointed to by data_0215fe8c. Read by link play code (LinkPhase_InitScreen,
 * map select, battle setup) through small getters. */
typedef struct Unk_0215fe8c {
    u8 pad_00[0x06];
    u16 unk_06;                        /* cleared by Link_ResetBuffers */
    u16 unk_08;                        /* func_0207a1dc result, stored when func_0207bb48 stops the task */
    u8 pad_0a[2];
    u8 unk_0c;                         /* func_0207bb48: 0 while the data_0215feb8 task runs, else 1 */
    u8 unk_0d;                         /* a player number, 0-3 (func_0207bd6c) */
    u8 unk_0e;                         /* packet counter, incremented by Link_SendPacket */
    u8 pad_0f[0x18 - 0x0f];
    u32 unk_18;                        /* set to 1 by Link_FlagError, e.g. when the event queue is full */
    u32 unk_1c;                        /* 1 in some link mode (func_0207bd80) */
} Unk_0215fe8c;

#endif
