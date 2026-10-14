#ifndef GAME_BATTLE_SAVE_H
#define GAME_BATTLE_SAVE_H

#include "types.h"
#include "game/player.h"
#include "game/map_unit.h"
#include "game/map_object.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022977a4.h"

#define BATTLE_SAVE_UNITS_PER_PLAYER 51
#define BATTLE_SAVE_TILE_COUNT 92
#define BATTLE_SAVE_OBJECT_COUNT 16
#define BATTLE_SAVE_TILES_END \
  0xffff /* BattleSaveTile.cell after the last changed tile */
#define BATTLE_SAVE_RECORD_SIZE \
  0x2238 /* bytes of data_02291774 written as one battle save record */

/* Unk_02080440_48 from gameMode (0x58) up to sidePanelRight (0xbc), copied
 * as one block. */
/* Bytes of gGameFlagsLow: one bit per flag id 0-31. */
#define GAME_FLAGS_LOW_SIZE 4

typedef struct BattleSaveSettings {
  union {
    u32 words[25];
    struct {
      u8 pad_00[0x30];
      u16 secondMapId; /* Unk_02080440_48.secondMapId */
    };
  };
} BattleSaveSettings;

/* Unk_02080440_48.unk_28 and unk_2a, copied as one block. */
typedef struct BattleSaveTilePos {
  s16 x;
  s16 y;
} BattleSaveTilePos;

/* A map tile whose cell code differs from the map data. */
typedef struct BattleSaveTile {
  u8 x;
  u8 y;
  u16 cell; /* MapTiles.mapCells value */
} BattleSaveTile;

/* The state of one map of a saved battle, 0x10b0 bytes. A two-map battle
 * keeps two of them in a row (BattleSave_Unpack reads them from data_02291774
 * + 0xd8 and + 0x1188). */
typedef struct BattleSaveMap {
  u16 currentPlayer;
  u16 day;
  BattleSaveTilePos unk_04; /* Unk_02080440_48.unk_28 and unk_2a */
  BattleSaveSettings settings;
  Unk_02080440_184 unk_6c;          /* Unk_02080440_48.unk_184 */
  Player players[PLAYER_SLOT_LAST]; /* player slots 1-4 */
  MapUnit units
      [PLAYER_SLOT_LAST *
       BATTLE_SAVE_UNITS_PER_PLAYER]; /* the first 51 unit slots of each player */
  Unk_02080440_5c unk_e34;            /* *Unk_02080440.unk_5c[i] */
  u8 pad_e3d;
  BattleSaveTile tiles
      [BATTLE_SAVE_TILE_COUNT]; /* ends at cell BATTLE_SAVE_TILES_END when not full */
  MapObject objects[BATTLE_SAVE_OBJECT_COUNT];
  u8 unk_102e[0x10b0 -
              0x102e]; /* filled by TileHp_Pack, read back by TileHp_Unpack */
} BattleSaveMap;

/* The unit slots of a game state, 64 per player. They start at 0x495c,
 * after the first 16 objects: Unk_02080440_48.objects is declared longer
 * than it is. */
#define STATE_UNIT_SLOTS(state) \
  ((MapUnit*)&(state)->objects[BATTLE_SAVE_OBJECT_COUNT])

typedef char BattleSaveMap_size_check[sizeof(BattleSaveMap) == 0x10b0 ? 1 : -1];

/* One skill set of 4 words, as in data_022a72c8. */
typedef struct BattleSaveSkillSet {
  u32 words[SKILL_SET_WORDS];
} BattleSaveSkillSet;

/* data_022a7b98, 0x40 bytes, copied whole. */
typedef struct BattleSaveBlock40 {
  u32 words[16];
} BattleSaveBlock40;

/* A whole battle save record as kept in data_02291774. */
typedef struct BattleSaveRecord {
  u8 unk_00; /* 0 or 1; nonzero sets the current state's unk_18 to 4 on load */
  u8 screen; /* data_027c0284 */
  u8 pad_02[2];
  u32 rngState;                         /* gRngState */
  u8 gameFlagsLow[GAME_FLAGS_LOW_SIZE]; /* gGameFlagsLow */
  Unk_022977a4 eventScript;             /* data_022977a4 */
  BattleSaveSkillSet
      lentSkills[PLAYER_SLOT_LAST]
                [PLAYER_CO_COUNT]; /* data_022a72c8 of players 1-4 */
  BattleSaveBlock40 unk_98;        /* data_022a7b98 */
  BattleSaveMap
      maps[2]; /* current screen, then the second map of a two-map battle */
} BattleSaveRecord;

typedef char BattleSaveRecord_size_check
    [sizeof(BattleSaveRecord) == BATTLE_SAVE_RECORD_SIZE ? 1 : -1];

#endif
