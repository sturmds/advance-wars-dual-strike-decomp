#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"

typedef struct Unk_ov004_022bf158_818 {
    u8 pad_00[0xc];
    u16 unk_0c;                        /* func_ov004_022b877c eases both from */
    u16 unk_0e;                        /* 256 to 128: maybe a scale */
} Unk_ov004_022bf158_818;

/* One 0x1c-byte element of Unk_ov004_022bf158.entries. */
typedef struct Ov004Entry {
    int active;                        /* set to 1 by Ov004_EntryAppear */
    u8 pad_04[4];
    int unk_08;                        /* eased from 31 to 0 by Ov004_EntryAppear */
    u8 pad_0c[8];
    int unk_14;                        /* eased from 2 to 64 */
    int unk_18;                        /* eased from 190 to 128 */
} Ov004Entry;

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
    u32 unk_000;
    u32 unk_004;
    u32 unk_008;
    u8 pad_00c[0x818 - 0xc];
    Unk_ov004_022bf158_818 unk_818;
    u8 pad_828[0x82c - 0x828];
    Ov004Entry entries[7];             /* count is a guess; indexed by the task parameter at 0x44 */
    u8 pad_8f0[0x8f8 - 0x8f0];
    int unk_8f8;                       /* eased each frame by func_ov004_022b0984 */
    int unk_8fc;                       /* eased each frame by func_ov004_022b06d0 and func_ov004_022b1e78 */
    int unk_900;                       /* eased each frame by func_ov004_022b061c */
    int unk_904;                       /* eased in two stages by Ov004_EaseUnk904Twice */
    int unk_908;                       /* eased each frame by func_ov004_022b1d38 */
    u8 pad_90c[0x918 - 0x90c];
    struct Ov004Task *slotTasks[32];        /* task of each slot, 0 when free */
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
    u8 pad_99c[0x1320 - 0x99c];
    void *unk_1320;                    /* set from data_ov004_022bf14c by Ov004Task_ResetState */
    u8 pad_1324[0x1328 - 0x1324];
    u32 unk_1328;                      /* set to 0x06860000, an LCDC VRAM address */
    u8 pad_132c[0x1330 - 0x132c];
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
