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
    u8 pad_828[0x8f8 - 0x828];
    int unk_8f8;                       /* eased each frame by func_ov004_022b0984 */
    int unk_8fc;                       /* eased each frame by func_ov004_022b06d0 and func_ov004_022b1e78 */
    int unk_900;                       /* eased each frame by func_ov004_022b061c */
    u8 pad_904[4];
    int unk_908;                       /* eased each frame by func_ov004_022b1d38 */
    u8 pad_90c[0x998 - 0x90c];
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
    u8 pad_99c[0x1330 - 0x99c];
    u32 bgCharBase;                    /* DISPCNT bits 24-26, set by Ov004_ApplyBgCharBase */
    u8 pad_1334[0x1bf0 - 0x1334];
    u32 unk_1bf0;
    u8 pad_1bf4[0x1c10 - 0x1bf4];
} Unk_ov004_022bf158;

/* Task run by overlay 4's script steps. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct Ov004Task {
    u8 pad_00[0x2c];
    struct Unk_ov004_022bf158 *state;  /* allocated by Ov004_AllocState */
    int slot;                          /* index into Unk_ov004_022bf158.unk_998 */
    int step;                          /* progress inside the current script step */
    u8 pad_38[0x3c - 0x38];
    int frame;                         /* frames elapsed in the current ease */
    int duration;                      /* length of the ease, in frames */
    int tweenFrames;                   /* length of the next slide, in frames */
    int unk_48;                        /* set to the same frame count as tweenFrames */
    int unk_4c;
    u8 pad_50[4];
    int tweenFrom;                     /* start and end values of the slide, */
    int tweenTo;                       /* eased by Math_Ease */
    int unk_5c;
    int unk_60;
} Ov004Task;

#endif
