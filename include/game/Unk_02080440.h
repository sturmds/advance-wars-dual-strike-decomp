#ifndef GAME_UNK_02080440_H
#define GAME_UNK_02080440_H

#include "types.h"
#include "game/player.h"
#include "game/map_unit.h"
#include "game/combatant.h"
#include "game/map_object.h"

/* Terrain and owner of every map tile, reached through
 * Unk_02080440_48.mapTiles. */
typedef struct MapTiles {
    u8 unitAt[0x508];                  /* slot in Unk_02080440_48.units of the unit on each tile; 0: none */
    u8 unk_508[0x508];                 /* per tile: a unit slot too (Strike_ScoreSite, AiAttack_Score); how it differs from unitAt is unknown */
    u8 pad_a10[0x1420 - 0xa10];
    u8 tiles[0x508];                   /* bits 0-4 terrain id, bits 5-7 owner */
    s8 unk_1928[0x508];                /* per tile: index into Unk_02080440_48.unk_6088 */
    s8 inSight[0x508];                 /* per tile; nonzero: the current player can see it (Map_IsTileVisible) */
    s8 unk_2338[0x508];                /* per tile; no code seen uses it yet */
    s8 unk_2840[0x508];                /* per tile; scratch range map for AiEmptyTransport_GoToFootUnit */
    s8 unk_2d48[0x508];                /* per tile; AiUnit_MoveToSafestTile skips negative tiles when gAiTurnState->stepFlags has AI_STEP_UNIT_AT_TARGET */
    u8 threatMax[0x508];               /* per tile: highest threat of one enemy unit that can move next to it (Ai_AddEnemyThreat) */
    u8 threatSum[0x508];               /* per tile: threat of enemy units in range, added up and capped at 255 */
    s8 unk_3c60[0x508];                /* per tile; AiEmptyTransport_GoToFootUnit fills it with the acting unit's moves */
    u16 rowStart[1];                   /* length unknown; index into tiles of each row's first tile */
} MapTiles;

#define MAP_TILE_TERRAIN(t) ((t) & 0x1f)
#define MAP_TILE_OWNER(t)   (((t) & 0xe0) >> 5)

/* Terrain ids (MAP_TILE_TERRAIN) are in game/terrain.h. */

/* Reached through Unk_02080440_48.unk_6088, 6 bytes; indexed by
 * MapTiles.unk_1928. */
typedef struct Unk_02080440_6088 {
    u8 unk_00;                         /* 0xff ends the table */
    u8 x;
    u8 y;
    u8 unk_03;                         /* raised when an AI foot unit heads for the tile to capture it */
    u8 unk_04;                         /* cleared with unk_03 and unk_05 at the start of an AI turn */
    u8 unk_05;
} Unk_02080440_6088;

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x1a];
    u16 mapWidth;                      /* in tiles */
    u16 mapHeight;
    u8 pad_1e[0x2a - 0x1e];
    u16 unk_2a;
    u8 pad_2c[0x30 - 0x2c];
    u32 unk_30;
    u16 mapId;                         /* index into Ov000_gMapHeaders */
    u8 pad_36[0x38 - 0x36];
    u8 fog;                            /* 1: Fog of War */
    u8 tileset;                        /* from MapHeader.tileset */
    u8 weather;                        /* WEATHER_* */
    u8 unk_3b;                         /* 1: menu weather setting 0 */
    u8 unk_3c;                         /* set by func_020ddb38; passed to func_020801dc */
    u8 unk_3d;                         /* 0-3, mapped to menu weather settings 1-4 */
} Unk_02080440_1c4;

/* Unk_02080440_1c4.weather */
#define WEATHER_CLEAR 0
#define WEATHER_SNOW  1
#define WEATHER_RAIN  2
#define WEATHER_SAND  3

typedef struct Unk_02080440_48 {
    u16 unk_00;                        /* compared with 1 by AiTurn_RunEndActions (likely the day) */
    u16 currentPlayer;                 /* index into players; CO power code works on this player (likely whose turn it is) */
    u16 unk_04;                        /* compared with a map tile's owner bits (tile & 0xe0), unshifted */
    u16 currentUnitBase;               /* (currentPlayer - 1) * MAP_UNITS_PER_PLAYER: first slot in units */
    u8 pad_08[0x0d - 0x08];
    u8 unk_0d;                         /* set to 1 by func_020b9890, 0 by func_020b9864, read by func_020b983c */
    u8 pad_0e[0x1e - 0x0e];
    u16 unk_1e;                        /* saved and restored around Ai_BuildThreatMaps */
    u8 pad_20[0x38 - 0x20];
    MapUnit *actingUnit;               /* &units[actingUnitIndex] */
    u8 actingUnitIndex;                /* slot in units of the unit the AI is moving */
    u8 pad_3d;
    s16 actingUnitStartX;              /* where the acting unit stood before it moved */
    s16 actingUnitStartY;
    s16 unk_42;
    s16 unk_44;
    u8 unk_46;                         /* passed to Unit_GetFuelUsed with actingUnit to get fuel used */
    u8 pad_47[0x58 - 0x47];
    u8 gameMode;                       /* values 0-3; meaning of each unconfirmed */
    u8 unk_59;
    u8 pad_5a[0x5d - 0x5a];
    u8 coEffectsOn;                    /* name is a guess; 0: no CO luck (good luck 10, bad luck 0) */
    u8 partnerBoostHuman;              /* nonzero: non-AI players get tag partner skills */
    u8 partnerBoostAi;                 /* nonzero: AI players get tag partner skills */
    u8 pad_60[0x78 - 0x60];
    s32 fundsPerProperty;              /* income from each city, base, airport, port and HQ */
    u32 unk_7c;
    u8 unk_80;                         /* from MapHeader.unk_30 */
    u8 unk_81;                         /* from MapHeader.unk_34 */
    u8 unk_82;
    u8 musicReady;                     /* 0: song requests are kept pending, see Sound_PlaySong; name a guess */
    u8 pad_84[0x85 - 0x84];
    u8 luckOnRequestOnly;              /* nonzero: Combatant_CalcDamage rolls luck only when its caller asks; name a guess */
    u8 unk_86;
    u8 pad_87[0x88 - 0x87];
    u16 secondMapId;                   /* second map of a two-map battle, from MapHeader.variantMapIds[0]; 0: none */
    u8 twoMaps;                        /* 1: a two-map battle (see secondMapId) */
    u8 pad_8b[0x8c - 0x8b];
    u8 unk_8c;
    u8 army[5];                        /* army colour per slot, copied from the map header */
    u8 slotControllers[5];             /* per slot: PLAYER_CONTROLLER_*, set from the game mode and map */
    u8 playerCos[5][2];                /* front and back CO id per slot; 0: none */
    u8 team[5];                        /* team per slot, copied from the map header */
    u8 pad_a6[0xba - 0xa6];
    u8 coSelectPhase;                  /* 1 on maps where a player picks a CO; turns off Scout and Eagle Eye, see Map_IsCoSelectPhase */
    u8 pad_bb[0xd0 - 0xbb];
    s8 *rangeMap[40];                  /* one s8 per tile, by row; filled for a unit by RangeMap_FillUnitMoves and RangeMap_MarkAdjacentTiles; the AI skips negative tiles */
    u8 unk_170[12]; /* 12 values of 0-15, see PackNibblePairs */
    u8 pad_17c[0x1c4 - 0x17c];
    Unk_02080440_1c4 *unk_1c4;
    u8 pad_1c8[0x43d0 - 0x1c8];
    MapTiles *mapTiles;
    u8 tileHp[0x508];                  /* per tile like MapTiles; bits 0-6: hp of a tile units can attack (pipe seams) */
    MapObject objects[0x1d0];          /* the list ends at the first object with kind 0 */
    MapUnit *units;                    /* MAP_UNITS_PER_PLAYER slots per player */
    u8 pad_5760[0x5a58 - 0x5760];
    Player *players;                   /* 5 slots: 0 neutral, then players 1-4 */
    u8 pad_5a5c[0x5e5c - 0x5a5c];
    u8 *tileCodeTerrain;               /* per map tile code: bits 0-4 terrain id */
    u8 pad_5e60[0x6088 - 0x5e60];
    Unk_02080440_6088 *unk_6088;
    u8 pad_608c[0x61a0 - 0x608c];
    Combatant combatants[2];           /* attacker, defender */
} Unk_02080440_48;

/* Reached through Unk_02080440.unk_5c[i]. */
typedef struct Unk_02080440_5c {
    u8 pad_00[3];
    u8 weatherDays[5];                 /* per player slot: days the current weather lasts; set by Weather_SetDays */
} Unk_02080440_5c;

/* Player slot that owns a unit in state->units: slot 0 is neutral, so the
 * first MAP_UNITS_PER_PLAYER (64) units belong to slot 1. */
#define MAP_UNIT_OWNER(state, unit) ((((unit) - (state)->units) >> 6) + 1)

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x2c];
    struct Task *taskPool;             /* TASK_COUNT tasks */
    struct Task **taskFreeStack;       /* TASK_COUNT + 1 entries, ends with NULL */
    struct Task **taskFreeTop;         /* next free task in taskFreeStack */
    struct Task **taskLists;           /* TASK_LEVELS lists of top-level tasks */
    struct ScriptSlot *scriptSlots;    /* 30 slots, see game/script_slot.h */
    u8 pad_40[0x48 - 0x40];
    Unk_02080440_48 *unk_48[5]; /* indexed by data_027c0284; length is a guess (unk_5c follows) */
    Unk_02080440_5c *unk_5c[1];        /* length unknown, indexed by data_027c0284 like unk_48 */
} Unk_02080440;

#endif
