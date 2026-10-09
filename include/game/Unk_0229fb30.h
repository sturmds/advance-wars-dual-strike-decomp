#ifndef GAME_UNK_0229FB30_H
#define GAME_UNK_0229FB30_H

#include "types.h"

/* data_0229fb30 (main bss). */
typedef struct Unk_0229fb30 {
    u8 pad_00[2];
    u8 unk_02;                         /* Battle_ShowQuote sets it to 1 */
    u8 unk_03;                         /* 1-3: which of three frame graphics the battle screen loads */
} Unk_0229fb30;

extern Unk_0229fb30 data_0229fb30;

#endif
