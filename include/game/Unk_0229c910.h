#ifndef GAME_UNK_0229C910_H
#define GAME_UNK_0229C910_H

#include "types.h"

/* data_0229c910 (main bss, 0x18 bytes). Link play. */
typedef struct Unk_0229c910 {
    u8 pad_00[0x14];
    u16 unk_14;                        /* set to 1 by func_0207a750 */
    u8 unk_16;                         /* copy of Unk_0229c960.unk_10 */
    u8 unk_17;                         /* copy of Unk_0215fe84.unk_35 */
} Unk_0229c910;

extern Unk_0229c910 data_0229c910;

#endif
