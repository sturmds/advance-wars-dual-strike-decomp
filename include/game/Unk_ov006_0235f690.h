#ifndef GAME_UNK_OV006_0235F690_H
#define GAME_UNK_OV006_0235F690_H

#include "types.h"

/* Overlay 6 state reached through data_ov006_0236d640. */
typedef struct Unk_ov006_0235f690 {
    u16 unk_00;                        /* flags; Ov006_OpenScreen sets 0x1000 */
    u8 pad_02[0x2c - 0x02];
    u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
} Unk_ov006_0235f690;

#endif
