#ifndef GAME_UNK_02160AC0_H
#define GAME_UNK_02160AC0_H

#include "types.h"

/* Pointed to by data_02160ac0 (main data). func_0208bf40 fills bytes
 * 0x00-0x13, 0x18, 0x19, 0x7d and 0x84 from data_0229e148. */
typedef struct Unk_02160ac0 {
    u8 pad_00[0x18];
    u8 unk_18;
    u8 unk_19;
    u8 pad_1a[0x7c - 0x1a];
    u8 unk_7c;                         /* nonzero: Ov003_InitState uses a fixed random seed */
    u8 unk_7d;
    u8 unk_7e;
    u8 pad_7f[0x83 - 0x7f];
    u8 unk_83;                         /* 0-4: picks the help text of entry 4 in LinkMenu_DrawHelpText */
    u8 unk_84;
} Unk_02160ac0;

extern Unk_02160ac0 *data_02160ac0;

#endif
