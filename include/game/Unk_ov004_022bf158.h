#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"
#include "game/affine_params.h"

#define OV004_SLOT_COUNT 32            /* task slots in the state */
#define OV004_TIMED_EVENT_COUNT 40     /* flags in timedEventFired */
#define OV004_EASE_COUNT 8             /* eased values from unk_8f8 on (unk_8f8 to unk_914) */
#define OV004_EASE_VALUES(state) (&(state)->unk_8f8)

/* One 0x1c-byte element of Unk_ov004_022bf158.entries. */
typedef struct Ov004Entry {
    int active;                        /* set to 1 by Ov004_EntryAppear */
    int unk_04;                        /* 0-6, set per entry by Ov004Task_SetupEntryScene */
    int unk_08;                        /* eased from 31 to 0 by Ov004_EntryAppear */
    int unk_0c;                        /* -128 to 64 in steps of 32: maybe an x position */
    int unk_10;                        /* -64 for every entry */
    int unk_14;                        /* eased from 2 to 64 */
    int unk_18;                        /* eased from 190 to 128 */
} Ov004Entry;

/* One 16-byte slot of Unk_ov004_022bf158.resources. */
typedef struct Ov004Resource {
    u32 unk_00;
    const char *name;                  /* matched by pointer or by string compare */
    void *data;
    u32 size;
} Ov004Resource;

/* One row of the list Ov004_LoadResources reads (data_ov004_022becf8). In
 * the "/syogun" directory, name is a code instead: OV004_RESOURCE_CO_* plus
 * a CO id. */
typedef struct Ov004ResourceFile {
    const char *dir;                   /* NULL ends the list */
    const char *name;
} Ov004ResourceFile;

#define OV004_RESOURCE_KIND_MASK 0xfffff000
#define OV004_RESOURCE_CO_PICTURE 0x1000
#define OV004_RESOURCE_CO_PALETTE 0x2000

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
    u32 unk_000;
    u32 unk_004;
    u32 unk_008;
    u32 unk_00c;                       /* passed by address to a task helper by Ov004Task_RunBannerTimeline */
    u8 pad_010[0x02c - 0x010];
    u32 unk_02c;                       /* passed by address to PaletteFade_StartFromBlack by Ov004Task_RunPictureQuadTimeline */
    u8 pad_030[0x10c - 0x030];
    u32 unk_10c;                       /* address passed to PaletteFade_StartFromBlack */
    u8 pad_110[0x810 - 0x110];
    struct Task *unk_810;              /* destroyed by Ov004_SkipOnInput */
    struct Task *unk_814;              /* destroyed by Ov004_SkipOnInput */
    AffineParams bg3Params;            /* first screen's BG3 (Ov004_ApplyBg3Affine) */
    Ov004Entry entries[7];             /* count is a guess; indexed by the task parameter at 0x44 */
    struct Task *controlTask;          /* made by Ov004_StartControlTask, 0 when none */
    u32 controlRunning;                /* 1 while controlTask runs; guess */
    int unk_8f8;                       /* eased each frame by func_ov004_022b0984 */
    int unk_8fc;                       /* eased each frame by func_ov004_022b06d0 and func_ov004_022b1e78 */
    int unk_900;                       /* eased each frame by func_ov004_022b061c */
    int unk_904;                       /* eased in two stages by Ov004_EaseUnk904Twice */
    int unk_908;                       /* eased each frame by func_ov004_022b1d38 */
    int unk_90c;                       /* eased like unk_908 by Ov004_EaseUnk908Twice */
    u8 pad_910[0x918 - 0x910];
    struct Ov004Task *slotTasks[OV004_SLOT_COUNT]; /* task of each slot, 0 when free */
    u32 unk_998[1];                    /* indexed by Ov004Task.slot; length unknown */
    u8 pad_99c[0xa18 - 0x99c];
    struct Task *slotTask[24];         /* tasks made by Ov004_CreateSlotTasks, one per slot */
    int slotState[24];                 /* per slot: 0 unused, 1 running, 2 set by func_ov004_022ba98c */
    struct Task *childTasks[16];       /* destroyed together by Ov004_DestroyChildTasks */
    int resourceCount;                 /* entries of resources in use, from Ov004_LoadResources */
    Ov004Resource resources[128];      /* looked up by Ov004_FindResource */
    u32 bufferUsed;                    /* bytes of unk_1320 taken by resources */
    void *unk_1320;                    /* set from data_ov004_022bf14c by Ov004Task_ResetState */
    u32 vramUsed;                      /* bytes from unk_1328 taken by resources */
    u32 unk_1328;                      /* set to 0x06860000, an LCDC VRAM address */
    u32 unk_132c;
    u32 bgCharBase;                    /* DISPCNT bits 24-26, set by Ov004_ApplyBgCharBase */
    u16 viewX;                         /* position of the scrolling tile view; eased by Ov004_EaseLayer1View */
    u16 viewY;
    u16 viewScrollX;                   /* viewX and viewY wrapped to the BG; set by func_ov004_022bacd8 */
    u16 viewScrollY;
    u16 blocksWide;                    /* set by Ov004_InitTileMap: size of the block tile map */
    u16 blocksHigh;
    void *tileMap;                     /* u16 entries; filled by Ov004_BlitBlock and Ov004_FillBlock */
    u16 mapBlock[32 * 32];             /* one unpacked 32x32 block of map entries */
    int elapsedFrames;                 /* frames since startTick, from Ov004_UpdateElapsedFrames */
    u64 startTick;                     /* clock reading taken by Ov004_StartClock */
    u32 timedEventFired[OV004_TIMED_EVENT_COUNT];           /* set once elapsedFrames passes the event's frame, see Ov004_CheckTimedEvent */
    u32 unk_1bf0;
    struct Task *unk_1bf4;             /* the six tasks from 0x1bf4 to 0x1c08 are destroyed by Ov004_DestroySceneTasks */
    struct Task *unk_1bf8;
    struct Task *unk_1bfc;
    struct Task *unk_1c00;
    struct Task *unk_1c04;
    struct Task *unk_1c08;
    int unk_1c0c;                      /* Ov004_QueueBottomSprites draws only while this is nonzero */
} Unk_ov004_022bf158;

/* Task run by overlay 4's script steps. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct Ov004Task {
    u8 pad_00[0x2c];
    struct Unk_ov004_022bf158 *state;  /* allocated by Ov004_AllocState */
    int slot;                          /* index into Unk_ov004_022bf158.unk_998 */
    int step;                          /* progress inside the current script step */
    int unk_38;
    int frame;                         /* frames elapsed in the current ease */
    int duration;                      /* length of the ease, in frames */
    int tweenFrames;                   /* length of the next slide, in frames */
    int unk_48;                        /* set to the same frame count as tweenFrames */
    int unk_4c;
    int unk_50;
    int tweenFrom;                     /* start and end values of the slide, */
    int tweenTo;                       /* eased by Math_Ease */
    int unk_5c;
    int unk_60;
    struct ShowSprite *sprite;         /* made by Ov004Task_InitSprite */
} Ov004Task;

/* One row of the table that Ov004_CreateSlotTasks copies into its new tasks
 * (data_ov004_022bea58): the values go to Ov004Task unk_38, tweenFrom,
 * tweenTo, tweenFrames, unk_48, unk_4c and unk_50. */
typedef struct Ov004SlotInit {
    int unk_38;
    int tweenFrom;
    int tweenTo;
    int tweenFrames;
    int unk_48;
    int unk_4c;
    int unk_50;
} Ov004SlotInit;

/* Setters of Unk_ov004_022bf158.bg3Params. */
void Ov004_SetBg3Center(Unk_ov004_022bf158 *state, u16 x, u16 y);
void Ov004_SetBg3Scale(Unk_ov004_022bf158 *state, u16 x, u16 y);
void Ov004_SetBg3Params(Unk_ov004_022bf158 *state, u32 srcX, u32 srcY, u16 centerX, u16 centerY, u16 scaleX, u16 scaleY, u16 angle);

#endif
