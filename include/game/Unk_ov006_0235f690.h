#ifndef GAME_UNK_OV006_0235F690_H
#define GAME_UNK_OV006_0235F690_H

#include "types.h"

/* One 12-byte slot; bit 0 of flags is tested by func_ov006_023547bc.
 * A slot belongs to a slot button: Ov006_AddSlotButtons treats the array as
 * starting at 0x78, so this view begins 8 bytes into each slot. */
typedef struct Ov006Slot {
  s16 flags;
  u8 pad_02;
  u8 state; /* slide animation: 10 start in, 11 sliding in, 20 shown,
                                           30 start out, 31 sliding out, 32 and 40 done */
  u8 pad_04[8];
} Ov006Slot;

/* The slot of one slot button, as Ov006_AddSlotButtons fills it. The button
 * slides in from x = -32 (fixed point, 12 fraction bits). */
typedef struct Ov006SlotButton {
  s32 x;
  s32 speed;
  s16 flags;
  u8 unk_0a; /* 32 for every slot */
  u8 state;  /* see Ov006Slot.state */
} Ov006SlotButton;

/* The design map slot buttons: MenuButton ids from OV006_SLOT_BUTTON_FIRST,
 * one per entry of slotButtons. */
#define OV006_SLOT_BUTTON_FIRST 32
#define OV006_SLOT_BUTTON_COUNT 8
#define OV006_SLOT_BUTTON_LAST \
  (OV006_SLOT_BUTTON_FIRST + OV006_SLOT_BUTTON_COUNT - 1)

/* The terrain page of the design maps screen: the terrain id of each entry,
 * 12 terrains and then the properties, HQ first, which terrainPage shows in
 * the colours of one army (Ov006_LoadArmyTiles). */
#define OV006_TERRAIN_ENTRY_COUNT 18
#define OV006_PROPERTY_ENTRY_FIRST 12
#define OV006_PROPERTY_ENTRY_COUNT \
  (OV006_TERRAIN_ENTRY_COUNT - OV006_PROPERTY_ENTRY_FIRST)

typedef struct Ov006TerrainEntries {
  s8 ids[OV006_TERRAIN_ENTRY_COUNT]; /* TerrainId */
  s8 end;                            /* -1; nothing reads it */
} Ov006TerrainEntries;

extern Ov006TerrainEntries data_ov006_0236a7a0;

/* Ov006Slot.state and Ov006SlotButton.state */
#define OV006_SLIDE_NONE 0
#define OV006_SLIDE_IN_START 10
#define OV006_SLIDE_IN 11
#define OV006_SLIDE_SHOWN 20
#define OV006_SLIDE_OUT_START 30
#define OV006_SLIDE_OUT 31
#define OV006_SLIDE_OUT_LAST \
  32 /* button 39 after sliding out: acts on chosenButton */
#define OV006_SLIDE_DONE 40

/* Overlay 6 state reached through data_ov006_0236d640. */
typedef struct Unk_ov006_0235f690 {
  u16 unk_00; /* flags; Ov006_OpenScreen sets 0x1000 */
  u16 unk_02; /* flags: 0x10 and 0x01 are tested by func_ov006_02351ac4 */
  u16 touch;  /* 0x8000 while touched; 1 or 2 on release (gTouch.prevPhase) */
  s16 touchX;
  s16 touchY;
  u16 request;     /* code set by Ov006_PostRequest (1, 2 or 4) */
  u16 screenState; /* step of the current request's screen (Ov006_UpdateArmyCoPanel, Ov006_UpdateTextPages, ...) */
  s16 cursorX;     /* editor cursor tile, the tile the tools act on */
  s16 cursorY;
  s16 prevCursorX; /* cursorX, cursorY of the last frame (DesignRoom_Update) */
  s16 prevCursorY;
  s16 viewX; /* tile the map view follows (Ov006_ScrollMapView) */
  s16 viewY;
  u16 toolUnit; /* unit mode tool: unit type in the low 6 bits */
  u16 toolTile; /* terrain mode tool: tile byte, terrain plus owner << 5 */
  u16 unk_1e;   /* entry index that toolUnit was set for */
  u16 unk_20;   /* entry index that toolTile was set for */
  u16 oldCell; /* cell code of the tile an edit is about to overwrite (Ov006_BreakCellBlock) */
  s8 unitArmy;        /* army (1-4) that unit mode places units for */
  s8 tileArmy;        /* owner of property tiles that terrain mode places */
  s8 prevTerrainPage; /* terrainPage before the last page change */
  s8 terrainPage; /* entry list page in terrain mode: army 0-4 whose properties it shows (Ov006_SetEntryPage) */
  s8 unitPage; /* entry list page in unit mode: army 1-4 whose CO icons and units it shows */
  s8 unitMode; /* 1: the editor places units; 0: it paints terrain */
  s8 toolResult; /* Ov006_CheckToolAtCursor: 1 allowed, 5 delete mode, 6 refused */
  s8 propertyCount; /* property tiles on the map (Ov006_CountPropertyTiles); at most 60 */
  u8 unk_2c[4]; /* Player_CountUnits(1..4): count of nonzero entries per slot */
  s8 hqX[4];    /* per army 1-4: HQ tile, -1 when none (Ov006_FindHqs) */
  s8 hqY[4];
  u8 chosenSlot; /* design map slot that Ov006_StartIdScript was called with; 0xff (-1) when none */
  u8 unk_39; /* set to 32, the id of the first slot button, by Ov006_InitSlotButtons */
  s8 squashStep; /* step in Ov006_gPanelScaleSteps of the OBJ matrix 0 squash (states 30-32 of Ov006_UpdateArmyCoPanel) */
  s8 unk_3b; /* frame countdown, set to 30 on a request; at 0 a script starts (Ov006_StartScriptAfterDelay) */
  s8 recountDelay; /* frames until Ov006_UpdateMapEditor recounts; 5 after a unit is removed */
  u8 pad_3d[0x40 - 0x3d];
  s32 pendingSe; /* sound effect for Ov006_UpdateMapEditor to play at the end of its frame; 0: none */
  s16 winLeft; /* window 0 rectangle that Ov006_UpdateArmyCoPanel animates, in pixels */
  s16 winTop;
  s16 winRight;
  s16 winBottom;
  s32 chosenButton; /* id of the slot button that was chosen, or -1 */
  s32 unk_50; /* last button id of a run starting at 64, removed by func_ov006_02355b44 */
  s32 unk_54;          /* sub screen layer 3 scroll y, 12 fraction bits */
  s32 unk_58;          /* sprite x, 12 fraction bits; eases to 0x12000 */
  struct Task* unk_5c; /* task of script data_ov006_0236a218 */
  struct Task* unk_60; /* task of script data_ov006_0236a1e0 */
  struct Task*
      unk_64;   /* task of script data_ov006_0236acc4 (func_ov006_02354444) */
  void* unk_68; /* file /mapedit/00a, loaded by Ov006_LoadMapeditGraphics */
  u8 pad_6c[0x70 - 0x6c];
  void* unk_70; /* sprite set drawn by Ov006_UpdateSubScreen */
  void* unk_74; /* file /mapedit/00c, loaded when request 6 opens */
  union {
    Ov006SlotButton slotButtons[OV006_SLOT_BUTTON_COUNT];
    struct {
      u8 pad_78[0x80 - 0x78];
      Ov006Slot unk_80[7]; /* an 8th slot starts at 0xd4 */
      s16 unk_d4;          /* flags of that 8th slot */
      u8 pad_d6[0xd8 - 0xd6];
    };
  };
  u8 unk_d8[0xeb - 0xd8];  /* passed to Ov006_HasNonSpace */
  u8 unk_eb;               /* its address is passed to DesignMap_CopyName */
  u8 pad_ec[0x10c - 0xec]; /* DesignRoom_Init allocates 0x10c bytes */
} Unk_ov006_0235f690;

extern Unk_ov006_0235f690* data_ov006_0236d640; /* the Design Room state */

#endif
