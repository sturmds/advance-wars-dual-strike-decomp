#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"

typedef struct Unk_ov004_022bf158_818 {
    u8 pad_00[0xc];
    u16 unk_0c;                        /* func_ov004_022b877c eases both from */
    u16 unk_0e;                        /* 256 to 128: maybe a scale */
} Unk_ov004_022bf158_818;

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
    u32 unk_000;
    u32 unk_004;
    u8 pad_008[0x818 - 0x8];
    Unk_ov004_022bf158_818 unk_818;
    u8 pad_828[0x998 - 0x828];
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
    u8 pad_99c[0x1bf0 - 0x99c];
    u32 unk_1bf0;
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
    int tweenTo;                       /* eased by Math_Ease */
} Ov004Task;

#endif
