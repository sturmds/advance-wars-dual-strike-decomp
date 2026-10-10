#ifndef GAME_UNK_02160ABC_H
#define GAME_UNK_02160ABC_H

#include "types.h"

/* Pointed to by data_02160abc (main data). */
typedef struct Unk_02160abc {
    u8 pad_00[0x0a];
    u8 unk_0a;                         /* Ov003_Update clears it before quitting from the pause menu */
} Unk_02160abc;

extern Unk_02160abc *data_02160abc;

#endif
