#ifndef GAME_UNK_02160AC0_H
#define GAME_UNK_02160AC0_H

#include "types.h"

/* Pointed to by data_02160ac0 (main data). */
typedef struct Unk_02160ac0 {
    u8 pad_00[0x83];
    u8 unk_83;                         /* 0-4: picks the help text of entry 4 in LinkMenu_DrawHelpText */
} Unk_02160ac0;

extern Unk_02160ac0 *data_02160ac0;

#endif
