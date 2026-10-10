#ifndef GAME_UNK_02031A14_H
#define GAME_UNK_02031A14_H

#include "types.h"

/* Task made by func_02031a14. The first 0x2c bytes belong to the task
 * system (Task_Create). */
typedef struct Unk_02031a14 {
    u8 pad_00[0x2c];
    u32 unk_2c;
    u32 unk_30;
    u8 pad_34[0x52 - 0x34];
    u16 unk_52;
} Unk_02031a14;

void func_02031a14(u32 unk_2c, u32 unk_30, int unk_52);

#endif
