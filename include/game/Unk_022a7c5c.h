#ifndef GAME_UNK_022A7C5C_H
#define GAME_UNK_022A7C5C_H

#include "types.h"

/* Object reached through data_022a7c5c (main bss); only the fields the
 * overlay 0 show tasks read are listed. */
typedef struct Unk_022a7c5c {
    u8 pad_00[0x5c];
    u32 unk_5c;                        /* picked for an odd player slot (ShowTask_GetSlotValue) */
    u32 unk_60;                        /* picked for an even player slot */
} Unk_022a7c5c;

extern Unk_022a7c5c *data_022a7c5c;

#endif
