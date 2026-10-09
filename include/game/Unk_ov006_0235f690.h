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
    s8 unk_29;                         /* 1 or 0: which of two layouts Ov006_UpdateEntryView updates */
    u8 pad_2a[0x2c - 0x2a];
    u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
    u8 pad_30[0x38 - 0x30];
    u8 unk_38;                         /* the id Ov006_StartIdScript was called with */
    u8 pad_39[0xd8 - 0x39];
    u8 unk_d8[0xeb - 0xd8];            /* passed to func_ov006_023586ac */
    u8 unk_eb;                         /* its address is passed to func_020d7b04 */
} Unk_ov006_0235f690;

#endif
