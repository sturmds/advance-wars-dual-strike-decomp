#ifndef GAME_UNK_0229FF30_H
#define GAME_UNK_0229FF30_H

#include "types.h"

/* data_0229ff30 (main bss). Its users sit next to the Combat setup code (link play). */
typedef struct Unk_0229ff30 {
    u8 pad_00[0x58];
    u16 playerMask;                    /* bit per link player 0-7; CombatSetup_Init sets it from Wireless_GetChildAids, with bit 0 always set */
} Unk_0229ff30;

extern Unk_0229ff30 data_0229ff30;

#endif
