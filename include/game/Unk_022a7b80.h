#ifndef GAME_UNK_022A7B80_H
#define GAME_UNK_022A7B80_H

#include "types.h"

/* data_022a7b80 (main bss). */
typedef struct Unk_022a7b80 {
    u8 pad_00[6];
    u8 unk_06;
    u8 unk_07;
    s32 unk_08;                        /* Mode_PrintVariantValue shows it by unk_07: 0 seconds as h:mm, 1 and 2 a number */
    u8 pad_0c[0x10 - 0x0c];
    s32 unk_10;                        /* points the win/lose screen shows in game mode 3 */
} Unk_022a7b80;

#endif
