#ifndef GAME_UNK_OV006_0235F690_H
#define GAME_UNK_OV006_0235F690_H

#include "types.h"

/* Overlay 6 state reached through data_ov006_0236d640. */
typedef struct Unk_ov006_0235f690 {
    u16 unk_00;                        /* flags; Ov006_OpenScreen sets 0x1000 */
    u8 pad_02[0x1a - 0x02];
    u16 unk_1a;
    u16 unk_1c;
    u16 unk_1e;                        /* entry index that unk_1a was set for */
    u16 unk_20;                        /* entry index that unk_1c was set for */
    u8 pad_22[0x24 - 0x22];
    s8 unk_24;
    s8 unk_25;
    u8 pad_26;
    s8 unk_27;
    s8 unk_28;
    s8 unk_29;                         /* 1 or 0: which of two layouts Ov006_SelectEntry updates */
    u8 pad_2a[0x2c - 0x2a];
    u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
} Unk_ov006_0235f690;

#endif
