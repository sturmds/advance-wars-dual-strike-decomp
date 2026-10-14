#ifndef GAME_OV003_UNIT_H
#define GAME_OV003_UNIT_H

#include "types.h"

/* One of the 16 32-byte entries in data_ov003_02369538 (overlay 3). A new
 * entry starts with hp 100; a free entry has hp 0. */
typedef struct Ov003Unit {
  s16 x; /* 128 per tile, plus 64 to centre it */
  s16 y;
  u16 unk_04;
  u16 lastTile; /* index of the map tile checked last (Ov003_UpdateTileClaims) */
  u16 safeTimer; /* frames left in which hits do no damage; the unit blinks */
  s16 prevX; /* x, y, unk_04 and hp of the previous frame (Ov003_SnapshotUnits) */
  s16 prevY;
  u16 prevUnk_04;
  u8 prevHp;
  u8 unk_11;
  u8 facing : 3; /* OV003_DIR_*: 4 for an odd Ov003Cell nibble, else 3 */
  u8 row : 5;    /* 1-based row (team) the unit belongs to */
  u8 kind; /* from the tile code of the cell it came from; 1 and 2 swap on terrain 3 */
  u8 hp; /* 100 = full; 0 = free entry */
  u8 unk_15;
  u8 walkCounter; /* counts steps; >> 3 picks the walk frame */
  u8 unk_17_0 : 6;
  u8 unk_17_6 : 1;
  u8 unk_17_7 : 1;
  u8 aiTimer;       /* frames counted by Ov003Unit_UpdateAi */
  u8 shotWait;      /* frames until the next burst */
  s8 target;        /* nearest unit (Ov003Unit_FindNearest), -1 for none */
  s8 scatterTarget; /* unit a scattered burst aims at */
  u8 unk_1c_0 : 1;
  u8 unk_1c_1 : 1;
  u8 lockedTarget : 6; /* unit bumped into by Ov003Unit_Move */
  u8 shotCount : 6;    /* shots fired in the current burst */
  u8 unk_1d_6 : 1;
  u8 unk_1d_7 : 1;
  u8 awake : 1;     /* a computer unit acts only once awake */
  u8 wakeGroup : 7; /* bit in Ov003State.unk_1de that wakes it */
  u8 wakeRange;     /* wakes when its target is closer, in tiles */
} Ov003Unit;

/* Ov003Unit.facing and the direction passed to Ov003Unit_Move. */
#define OV003_DIR_UP 1
#define OV003_DIR_DOWN 2
#define OV003_DIR_LEFT 3
#define OV003_DIR_RIGHT 4

typedef char Ov003Unit_size_check[sizeof(Ov003Unit) == 0x20 ? 1 : -1];

/* 20-byte entry of data_ov003_02364d88, indexed by Ov003Unit.tile. */
typedef struct Ov003TileInfo {
  u16 unk_00; /* divisor for Ov003Unit.unk_04 in Ov003_DrawUnitAnims */
  u8 pad_02[2];
  u8 reach;       /* in tiles; an Ov003Marker sees units within reach + 2 */
  u8 burstLength; /* shots per burst (Ov003Unit.shotCount, Ov003Marker.burstCount) */
  u8 burstWait; /* frames between bursts (Ov003Unit.shotWait, Ov003Marker.cooldown) */
  u8 pad_07[0x14 - 0x07];
} Ov003TileInfo;

/* 2-byte cell of a 31-cell row in data_ov003_02364928 (62 bytes per row). */
typedef struct Ov003Cell {
  u8 kind : 6;  /* Ov003Unit.kind of the unit created here */
  u8 state : 2; /* 2: an Ov003Unit sits here */
  s8 unit;      /* index in data_ov003_02369538, -1 for none */
} Ov003Cell;

/* Five 62-byte rows at the start of Ov003State: 20 cells, then the row's
 * settings. Ov003_InitRows clears 0x28-0x39. */
typedef struct Ov003Row {
  Ov003Cell cells[20];
  u8 unk_28[10];
  u16 charge;     /* filled by Ov003Row_AddCharge, up to a limit per CO */
  u16 powerTimer; /* not 0 while the CO power runs */
  u16 slowTimer;  /* halves the row's unit speed while not 0 (pickup kind 5) */
  u8 unk_38;
  u8 unk_39;
  u8 spawnCount;
  u8 co
      : 4; /* CO choice (CombatCo_IdFromNumber gives the CO id); picks the power tables; 8-10 are resolved by Ov003_InitRows */
  u8 unk_3b_4 : 4; /* even number per active row, then shifted to centre them */
  u8 color : 7;    /* 1-based: picks this row's palette colours and tiles */
  u8 active : 1;
  u8 pad_3d;
} Ov003Row;

typedef struct Ov003Slot {
  u8 cursor;    /* cell of its row under the placing cursor */
  u8 row : 4;   /* 1 + row of cells; bit 0 picks Ov003Unit.facing */
  u8 state : 4; /* OV003_SLOT_*: what Ov003_RunSlotInput runs for it */
  u8 pad_02[2];
} Ov003Slot;

/* Ov003Slot.state, as Ov003_RunSlotInput dispatches it. */
#define OV003_SLOT_STATE_1 1 /* Ov003Unit_HandleInput runs the slot */
#define OV003_SLOT_PLACING \
  2 /* Ov003Slot_UpdatePlacing: cursor moves, a button places a unit */
#define OV003_SLOT_SCROLLING 3 /* Ov003_ScrollWithDpad */
#define OV003_SLOT_PLACED 4    /* unit created; nothing runs */

/* 5 bytes at Ov003State.spawns: where a unit appears and how it starts. */
typedef struct Ov003Spawn {
  u8 x; /* passed to Ov003_FindOpenTileNear */
  u8 y;
  u8 unk_2_0 : 3; /* becomes Ov003Unit.facing */
  u8 unk_2_3 : 3; /* bit 0 becomes Ov003Unit.awake */
  u8 unk_2_6 : 1; /* becomes Ov003Unit.unk_1d_7 */
  u8 unk_2_7 : 1; /* becomes Ov003Unit.unk_1d_6 */
  u8 unk_3;       /* becomes Ov003Unit.wakeRange */
  u8 unk_4;       /* becomes Ov003Unit.wakeGroup */
} Ov003Spawn;

/* 6-byte record in a spawn list that ends with a record whose tag is 0.
 * Ov003_LoadSpawns turns each one into an Ov003Spawn. */
typedef struct Ov003SpawnDef {
  u8 tag; /* cell tile written for it; 0 ends the list */
  u8 x;
  u8 y;
  u8 flags_0 : 3; /* becomes Ov003Spawn.unk_2_0 */
  u8 flags_3 : 3; /* becomes Ov003Spawn.unk_2_3 */
  u8 flags_6 : 1; /* becomes Ov003Spawn.unk_2_6 */
  u8 flags_7 : 1; /* becomes Ov003Spawn.unk_2_7 */
  u8 unk_4;
  u8 unk_5;
} Ov003SpawnDef;

/* 20-byte entry of data_ov003_02364e50, picked by data_02160ac0->unk_7d:
 * how Ov003_InitRows sets up the rows. */
typedef struct Ov003Preset {
  u8 pad_00[4];
  u32 unk_04;   /* passed to Ov003_LoadSpawns */
  u8 colors[4]; /* Ov003Row.color per row */
  u8 unk_0c[4]; /* Ov003Row.co per row */
  u8 rowCount : 3;
  u8 unk_10_3 : 1;     /* default for Ov003State.unk_1df_0 */
  u8 unk_10_4 : 4;     /* default for Ov003State.unk_1df_2 */
  u8 unk_11;           /* default for Ov003State.unk_1dc */
  u8 timeLimitMinutes; /* 0: no time limit (Ov003_DrawTimeLeft) */
  u8 pad_13;
} Ov003Preset;

/* Overlay 3 state, through data_ov003_02364928. */
typedef struct Ov003State {
  Ov003Row rows[5];
  u8 pad_136[0x138 - 0x136];
  Ov003Slot slots[7]; /* length unknown */
  u8 pad_154[0x156 - 0x154];
  Ov003Spawn spawns[1]; /* length unknown */
  u8 pad_15b[0x1bc - 0x15b];
  Ov003Preset* preset;
  u8 unk_1c0[8];
  u32 frameCount; /* frames on which Ov003_IsPhase0OrAbove6 returns 1 */
  u32 unk_1cc;
  s16 scrollX; /* map scroll in pixels */
  s16 scrollY;
  u8 activeRows; /* bit per row with Ov003Row.active set */
  u8 currentSlot;
  union {
    u16 unk_1d6;
    struct {
      u8 pad_1d6;
      u8 unk_1d7; /* nonzero: Ov003_FadeEvyStep and func_ov003_0235ce7c wait */
    };
  };
  u8 rowCount; /* rows in use */
  u8 unk_1d9;
  union {
    u8 phases;
    struct {
      u8 phase
          : 4; /* phase this frame runs in (Ov003_Update); 1: the row of the current slot (or Ov003State.row) is picked */
      u8 nextPhase
          : 4; /* phase asked for; Ov003_AdvancePhase copies it into phase */
    };
  };
  u8 row : 7; /* 1 to 5: the row spawns go to */
  u8 unk_1db_7 : 1;
  u8 unk_1dc;
  u8 nextSpawn; /* index of the next record in spawns and cell in the row */
  u8 unk_1de;
  u8 unk_1df_0 : 1;
  u8 unk_1df_1 : 1;
  u8 unk_1df_2 : 3;
  u8 unk_1df_5 : 1;
  u8 unk_1df_6 : 1;
  u8 pauseCursor : 1; /* pause menu: 0 top button, 1 bottom button */
} Ov003State;

/* Cast view of the slots from pad_136: each slot's u16 cooldown sits in the
 * 2 bytes before its Ov003Slot, so the slot record really starts at 0x136. */
typedef struct Ov003SlotCooldown {
  u16 cooldown;
  u8 pad_02[2];
} Ov003SlotCooldown;

#define OV003_SLOT_COOLDOWN(state, slot) \
  (((Ov003SlotCooldown*)(state)->pad_136)[slot].cooldown)

/* Map of overlay 3, through data_ov003_02364a88. */
#define OV003_MAP_TILE_MAX 0x508

typedef struct Ov003Map {
  u8 width;
  u8 height;
  u16 cells[OV003_MAP_TILE_MAX]; /* tile code per tile, row by row */
  u8 marks[OV003_MAP_TILE_MAX]; /* per tile: MARK_* bits (Ov003_MarkRowTiles) */
  u8 unk_f1a
      [OV003_MAP_TILE_MAX]; /* per tile: 100 for terrain 3, 16, 21 and 26, else 0 (Ov003_LoadMap) */
} Ov003Map;

/* This frame's touch and key input, copied by Ov003_ReadInput into
 * data_ov003_02365b04. */
typedef struct Ov003Input {
  u8 phase;  /* TouchState.phase */
  u8 active; /* low byte of TouchState.active */
  u8 x;
  u8 y;
  u16 keys; /* Keypad.held */
} Ov003Input;

extern Ov003State* data_ov003_02364928;
extern Ov003Map* data_ov003_02364a88;

/* Frames left on a slot's cursor (halfword at 0x136 + 4 * slot, just before
 * Ov003Slot.unk_00). */
#define OV003_SLOT_TIMER(slot) \
  (((u16*)((u8*)data_ov003_02364928 + 0x136))[(slot) * 2])

/* Byte 0x1d7 of Ov003State: the high byte of unk_1d6 (a halfword that is also
 * passed to func_02027aa0). */
#define OV003_UNK_1D7(state) (((u8*)&(state)->unk_1d6)[1])

extern Ov003Map data_ov003_02368110; /* what data_ov003_02364a88 points to */

extern Ov003State data_ov003_02365b14; /* what data_ov003_02364928 points to */

#endif
