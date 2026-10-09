#ifndef GAME_UNK_0215FE84_H
#define GAME_UNK_0215FE84_H

#include "types.h"

/* Pointed to by data_0215fe84 (main data). */
typedef struct Unk_0215fe84 {
    u8 pad_00[0x18];
    int unk_18;                        /* returned by func_02077f20 */
    u8 pad_1c[0x30 - 0x1c];
    int unk_30;                        /* returned by func_0207a170 */
} Unk_0215fe84;

extern Unk_0215fe84 *data_0215fe84;

#endif
