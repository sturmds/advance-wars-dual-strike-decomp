#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
    u8 pad_000[0x998];
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
} Unk_ov004_022bf158;

/* Task run by overlay 4's script steps. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct Ov004Task {
    u8 pad_00[0x30];
    int slot;                          /* index into Unk_ov004_022bf158.unk_998 */
} Ov004Task;

#endif
