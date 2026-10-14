#ifndef GAME_UNK_OV004_022BF158_H
#define GAME_UNK_OV004_022BF158_H

#include "types.h"
#include "game/affine_params.h"

#define OV004_SLOT_COUNT 32      /* task slots in the state */
#define OV004_SLOT_TASK_COUNT 24 /* slotTask and slotState */
#define OV004_CHILD_TASK_COUNT 16
#define OV004_RESOURCE_COUNT 128
/* Tiles on a side of a map block (mapBlock, Ov004_BlitBlock, Ov004_FillBlock). */
#define OV004_BLOCK_TILES 32
#define OV004_TIMED_EVENT_COUNT 40 /* flags in timedEventFired */
/* Bytes of the three work buffers Ov004_AllocState hands out: the resource
 * buffer (buffer), the BG3 picture buffer (data_ov004_022bf150) and the tile
 * map (tileMap). */
#define OV004_BUFFER_SIZE 0x80000
#define OV004_BG3_BUFFER_SIZE 0x10000
#define OV004_TILE_MAP_SIZE 0x4800
/* Where the sprite graphics start in buffer, after the textures. */
#define OV004_SPRITE_GRAPHICS_OFFSET 0x8000
#define OV004_EASE_COUNT \
  8 /* eased values from unk_8f8 on (unk_8f8 to unk_914) */
#define OV004_EASE_VALUES(state) (&(state)->unk_8f8)

/* One 0x1c-byte element of Unk_ov004_022bf158.entries. */
typedef struct Ov004Entry {
  int active;  /* set to 1 by Ov004_EntryAppear */
  int texSlot; /* 0-6: texture at 0x10000 + texSlot * 0x1000 */
  int shade;   /* grey level 0-31; eased from 31 to 0 by Ov004_EntryAppear */
  int x;       /* -128 to 64 in steps of 32 */
  int y;       /* -64 for every entry */
  int width;   /* eased from 2 to 64 */
  int height;  /* eased from 190 to 128 */
} Ov004Entry;

/* One 16-byte slot of Unk_ov004_022bf158.resources. */
typedef struct Ov004Resource {
  const char* dir;
  const char* name; /* matched by pointer or by string compare */
  void* data;
  u32 size;
} Ov004Resource;

/* One row of the list Ov004_LoadResources reads (data_ov004_022becf8). In
 * the "/syogun" directory, name is a code instead: OV004_RESOURCE_CO_* plus
 * a CO id. */
typedef struct Ov004ResourceFile {
  const char* dir; /* NULL ends the list */
  const char* name;
} Ov004ResourceFile;

#define OV004_RESOURCE_KIND_MASK 0xfffff000
#define OV004_RESOURCE_CO_PICTURE 0x1000
#define OV004_RESOURCE_CO_PALETTE 0x2000

/* Overlay 4 state reached through data_ov004_022bf158. */
typedef struct Unk_ov004_022bf158 {
  u32 unk_000;
  u32 unk_004;
  u32 unk_008;
  u16 savedPalette
      [0x200]; /* copy of the BG and OBJ palettes (16 colours per palette); PaletteFade_StartFromBlack fades to it */
  u8 pad_40c[0x810 - 0x40c];
  struct Task* unk_810;   /* destroyed by Ov004_SkipOnInput */
  struct Task* unk_814;   /* destroyed by Ov004_SkipOnInput */
  AffineParams bg3Params; /* first screen's BG3 (Ov004_ApplyBg3Affine) */
  Ov004Entry
      entries[7]; /* count is a guess; indexed by the task parameter at 0x44 */
  struct Task* controlTask; /* made by Ov004_StartControlTask, 0 when none */
  u32 controlRunning;       /* 1 while controlTask runs; guess */
  int unk_8f8;              /* eased each frame by func_ov004_022b0984 */
  int unk_8fc; /* eased each frame by func_ov004_022b06d0 and func_ov004_022b1e78 */
  int unk_900; /* eased each frame by func_ov004_022b061c */
  int unk_904; /* eased in two stages by Ov004_EaseUnk904Twice */
  int unk_908; /* eased each frame by func_ov004_022b1d38 */
  int unk_90c; /* eased like unk_908 by Ov004_EaseUnk908Twice */
  u8 pad_910[0x918 - 0x910];
  struct Ov004Task*
      slotTasks[OV004_SLOT_COUNT]; /* task of each slot, 0 when free */
  u32 unk_998[1]; /* indexed by Ov004Task.slot; length unknown */
  u8 pad_99c[0xa18 - 0x99c];
  struct Task* slotTask
      [OV004_SLOT_TASK_COUNT]; /* tasks made by Ov004_CreateSlotTasks, one per slot */
  int slotState
      [OV004_SLOT_TASK_COUNT]; /* per slot: 0 unused, 1 running, 2 set by func_ov004_022ba98c */
  struct Task* childTasks
      [OV004_CHILD_TASK_COUNT]; /* destroyed together by Ov004_DestroyChildTasks */
  int resourceCount; /* entries of resources in use, from Ov004_LoadResources */
  Ov004Resource
      resources[OV004_RESOURCE_COUNT]; /* looked up by Ov004_FindResource */
  u32 bufferUsed;                      /* bytes of buffer taken by resources */
  void* buffer; /* 0x80000 bytes, data_ov004_022bf14c (Ov004Task_ResetState) */
  u32 vramUsed; /* bytes from vramArea taken by resources */
  u32 vramArea; /* 0x20000 bytes from 0x06860000, an LCDC VRAM address */
  u32 bg3InSecondHalf; /* 1: Ov004_ShowLoadedBg3 last filled the first half, the next goes to the second */
  u32 bgCharBase;      /* DISPCNT bits 24-26, set by Ov004_ApplyBgCharBase */
  u16 viewX; /* position of the scrolling tile view; eased by Ov004_EaseLayer1View */
  u16 viewY;
  u16 viewScrollX; /* viewX and viewY wrapped to the BG; set by Ov004_CopyViewToBgMap */
  u16 viewScrollY;
  u16 blocksWide; /* set by Ov004_InitTileMap: size of the block tile map */
  u16 blocksHigh;
  void*
      tileMap; /* u16 entries; filled by Ov004_BlitBlock and Ov004_FillBlock */
  u16 mapBlock[OV004_BLOCK_TILES *
               OV004_BLOCK_TILES]; /* one unpacked 32x32 block of map entries */
  int elapsedFrames; /* frames since startTick, from Ov004_UpdateElapsedFrames */
  u64 startTick;     /* clock reading taken by Ov004_StartClock */
  u32 timedEventFired
      [OV004_TIMED_EVENT_COUNT]; /* set once elapsedFrames passes the event's frame, see Ov004_CheckTimedEvent */
  u32 unk_1bf0;
  struct Task*
      unk_1bf4; /* the six tasks from 0x1bf4 to 0x1c08 are destroyed by Ov004_DestroySceneTasks */
  struct Task* unk_1bf8;
  struct Task* unk_1bfc;
  struct Task* unk_1c00;
  struct Task* unk_1c04;
  struct Task* unk_1c08;
  int unk_1c0c; /* Ov004_QueueBottomSprites draws only while this is nonzero */
} Unk_ov004_022bf158;

/* Task run by overlay 4's script steps. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct Ov004Task {
  u8 pad_00[0x2c];
  struct Unk_ov004_022bf158* state; /* allocated by Ov004_AllocState */
  int slot;                         /* index into Unk_ov004_022bf158.unk_998 */
  int step; /* progress inside the current script step */
  int unk_38;
  int frame;       /* frames elapsed in the current ease */
  int duration;    /* length of the ease, in frames */
  int tweenFrames; /* length of the next slide, in frames */
  int unk_48;      /* set to the same frame count as tweenFrames */
  int unk_4c;
  int unk_50;
  int tweenFrom; /* start and end values of the slide, */
  int tweenTo;   /* eased by Math_Ease */
  int unk_5c;
  int unk_60;
  struct ShowSprite* sprite; /* made by Ov004Task_InitSprite */
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
void Ov004_SetBg3Center(Unk_ov004_022bf158* state, u16 x, u16 y);
void Ov004_SetBg3Scale(Unk_ov004_022bf158* state, u16 x, u16 y);
void Ov004_SetBg3Params(Unk_ov004_022bf158* state, u32 srcX, u32 srcY,
                        u16 centerX, u16 centerY, u16 scaleX, u16 scaleY,
                        u16 angle);

#endif
