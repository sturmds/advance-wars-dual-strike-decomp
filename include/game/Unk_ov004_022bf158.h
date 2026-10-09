#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
    u8 pad_000[0x4];
    u32 unk_004;
    u8 pad_008[0x998 - 0x8];
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
} Unk_ov004_022bf158;

/* Task run by overlay 4's script steps. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct Ov004Task {
    u8 pad_00[0x30];
    int slot;                          /* index into Unk_ov004_022bf158.unk_998 */
    int step;                          /* progress inside the current script step */
    u8 pad_38[0x44 - 0x38];
    int tweenFrames;                   /* length of the next slide, in frames */
    u8 pad_48[0x54 - 0x48];
    int tweenFrom;                     /* start and end values of the slide, */
    int tweenTo;                       /* eased by func_02026a60 */
} Ov004Task;

#endif
