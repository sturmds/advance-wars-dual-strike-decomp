#ifndef GAME_UNK_022A7B80_H
#define GAME_UNK_022A7B80_H

#include "types.h"

/* data_022a7b80 (main bss). */
typedef struct Unk_022a7b80 {
    u8 coIds[2];                       /* front and back CO of the run's player; Survival_SavePlayerCos fills them from the state, Survival_LoadPlayerCos puts them back */
    u16 powerCharge[2];                /* starting power meter of slot 1's front and back CO, with coIds; Survival_LoadPlayerCos copies it to Unk_02080440_48.unk_aa */
    u8 unk_06;
    u8 unk_07;
    s32 unk_08;                        /* Mode_PrintVariantValue shows it by unk_07: 0 seconds as h:mm, 1 and 2 a number */
    s32 unk_0c;                        /* the mode's full budget (func_020eaa8c), set when a run starts; unk_08 counts it down */
    s32 unk_10;                        /* points the win/lose screen shows in game mode 3 */
} Unk_022a7b80;

#endif
