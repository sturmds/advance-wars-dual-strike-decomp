#ifndef GAME_UNK_0229C734_H
#define GAME_UNK_0229C734_H

#include "types.h"

/* Pointed to by data_0229c734 (main bss). */
typedef struct Unk_0229c734 {
    u8 pad_00[0x9a];
    s16 unk_9a;                        /* set by func_0203813c, read by func_02038150 */
    u8 pad_9c[0x9d - 0x9c];
    u8 unk_9d;                         /* set by func_0203b194 */
    u8 pad_9e[0xa4 - 0x9e];
    int unk_a4;                        /* set to 1 by ScriptTask_ResumeWithValue, read by func_02038164 */
} Unk_0229c734;

extern Unk_0229c734 *data_0229c734;

#endif
