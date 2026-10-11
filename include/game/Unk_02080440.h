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
    u16 mapCells[0x508];               /* per tile: the cell code from the map data; tiles[] holds the converted byte (Map_LoadIntoState) */
    u8 tiles[0x508];                   /* bits 0-4 terrain id, bits 5-7 owner */
    s8 unk_1928[0x508];                /* per tile: index into Unk_02080440_48.unk_6088 */
    s8 inSight[0x508];                 /* per tile; nonzero: the current player can see it (Map_IsTileVisible) */
    s8 unk_2338[0x508];                /* per tile; nonzero: fog or rain does not hide it, so actions there are shown (unit capture, the AI show steps, UnitShowTask_CheckVisible); how it differs from inSight is unknown */
    s8 rangeMapTiles[0x508];           /* per tile; the buffer Unk_02080440_48.rangeMap points into by default (RangeMap_SetBuffer) */
    s8 rangeScratch[0x508];            /* per tile; a range map the AI fills for another unit (a transport's move cost, another unit's reach); AiUnit_MoveToSafestTile skips negative tiles when gAiTurnState->stepFlags has AI_STEP_UNIT_AT_TARGET */
    u8 threatMax[0x508];               /* per tile: highest threat of one enemy unit that can move next to it (Ai_AddEnemyThreat) */
    u8 threatSum[0x508];               /* per tile: threat of enemy units in range, added up and capped at 255 */
    s8 rangeScratch2[0x508];           /* per tile; a third range map (AiEmptyTransport_GoToFootUnit fills it with the acting unit's moves); AiTransport_UnloadForCapture reads bits 0x80 and 0x40 as Infantry and Mech drop flags */
    u16 rowStart[1];                   /* one per map row (Map_BuildRowStarts), indexed past [1]; the real length is unknown. Index into tiles of each row's first tile */
} MapTiles;

#define MAP_TILE_TERRAIN(t) ((t) & 0x1f)
#define MAP_TILE_OWNER(t)   (((t) & 0xe0) >> 5)
#define MAP_TILE_MAKE(terrain, owner) ((terrain) | ((owner) << 5))
#define MAP_TILE(terrain, owner) ((terrain) | ((owner) << 5))

/* Terrain ids (MAP_TILE_TERRAIN) are in game/terrain.h. */

/* Reached through Unk_02080440_48.unk_6088, 6 bytes; indexed by
 * MapTiles.unk_1928. */
typedef struct Unk_02080440_6088 {
    u8 unk_00;                         /* 0xff ends the table */
    u8 x;
    u8 y;
    u8 claimCount;                     /* AI units heading for the tile to capture it; cleared at the start of an AI turn */
    u8 unk_04;                         /* cleared with claimCount and unk_05 at the start of an AI turn; AiFindCaptureSiteTile reads unk_04 (direct units) or unk_05 (indirect) */
    u8 unk_05;
} Unk_02080440_6088;

/* Entry of Unk_02080440_48.unk_608c, 3 bytes. */
typedef struct Unk_02080440_608c {
    u8 tile;                           /* the MapTiles.tiles byte: terrain and owner */
    u8 x;
    u8 y;
} Unk_02080440_608c;

/* Reached through Unk_02080440.unk_48[i]->unk_1c4. */
typedef struct Unk_02080440_1c4 {
    u8 pad_00[0x1a];
    u16 mapWidth;                      /* in tiles */
    u16 mapHeight;
    s16 scrollX;                       /* view scroll in 1/16 tiles */
    s16 scrollY;
    u16 unk_22;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u16 scrollFlags;                   /* scroll directions in progress: 1 right, 2 left, 4 up, 8 down (MapView_ScrollStep) */
    u8 playerCount;                    /* players on the loaded map; the loop bound in func_02031498 */
    u8 pad_2d[0x30 - 0x2d];
    u32 unk_30;
    u16 mapId;                         /* index into Ov000_gMapHeaders */
    u8 pad_36;
    u8 unk_37;                         /* 3 after a rules reset */
    u8 fog;                            /* 1: Fog of War */
    u8 tileset;                        /* from MapHeader.tileset */
    u8 weather;                        /* WEATHER_* */
    u8 unk_3b;                         /* 1: menu weather setting 0 */
    u8 nextWeather;                    /* WEATHER_* for the next turn: func_020ddb38 picks it through func_020d29c0 (random when unk_3b is 1); AiTurn_Restart passes it to Link_SendWeather */
    u8 unk_3d;                         /* 0-3, mapped to menu weather settings 1-4 */
} Unk_02080440_1c4;

/* Unk_02080440_1c4.weather */
#define WEATHER_CLEAR 0
#define WEATHER_SNOW  1
#define WEATHER_RAIN  2
#define WEATHER_SAND  3

/* data_027c027c->unk_48[data_027c0284], written so that the compiler adds the
 * scaled index before the field offset; needed to match in some places. The
 * including file declares data_027c027c and data_027c0284. */
#define STATE_IDX_FIRST (*(Unk_02080440_48 **)((u8 *)data_027c027c + data_027c0284 * sizeof(Unk_02080440_48 *) \
                                    + (u32)&((Unk_02080440 *)0)->unk_48))

/* Which of the two map states is current: data_02183b08 (the first) or the
 * other one. func_020f5344 takes it. Written as `state == &data_02183b08 ?
 * MAP_STATE_FIRST : MAP_STATE_SECOND`: with enum constants mwcc keeps the
 * cmp/moveq/movne shape, where 0 and 1 would fold into `!=`. */
enum {
    MAP_STATE_FIRST,
    MAP_STATE_SECOND
};

/* Unk_02080440_48.gameMode values the code confirms. */
#define GAME_MODE_CAMPAIGN 0           /* Mission_HasPendingEvent runs only here */
#define GAME_MODE_WAR_ROOM 1           /* MapSelect_LoadWarRoomRecords runs only here */

typedef struct Unk_02080440_48 {
    u16 day;                           /* current day; map event triggers compare it (EventTriggers_Run) */
    u16 currentPlayer;                 /* index into players; CO power code works on this player (likely whose turn it is) */
    u16 unk_04;                        /* compared with a map tile's owner bits (tile & 0xe0), unshifted */
    u16 currentUnitBase;               /* (currentPlayer - 1) * MAP_UNITS_PER_PLAYER: first slot in units */
    s8 menuDepth;                      /* guess: menus open at once; +1 by MenuDepth_Push, -1 (not below 0) by MenuDepth_Pop */
    u8 pad_09[0x0d - 0x09];
    u8 unk_0d;                         /* set to 1 by func_020b9890, 0 by func_020b9864, read by func_020b983c */
    u8 pad_0e[0x10 - 0x0e];
    u32 unk_10;                        /* passed to PopupMenu_Create when the battle menu opens (BattleMenu_Open) */
    u32 unk_14;
    u16 unk_18;                        /* AiShowAction_Step0 tests 16 and 21 */
    u16 unk_1a;                        /* unk_18 before Game_EnterState18 last set it to 18 */
    u16 unk_1c;                        /* cleared when a unit move is cancelled (UnitMoveSelect_HandleButtons) */
    u16 unk_1e;                        /* saved and restored around Ai_BuildThreatMaps */
    u16 unk_20;                        /* index of a 0x508-byte per-tile grid counted from MapTiles.inSight; func_020fed58 fills it */
    u16 unk_22;                        /* a second grid index, like unk_20 */
    u8 pad_24[0x28 - 0x24];
    s16 unk_28;                        /* x of the tile the AiShowAction steps point Map_ScrollViewToTile at; guess: the view scrolls there */
    s16 unk_2a;                        /* y of the same tile */
    u16 unk_2c;
    u16 unk_2e;
    s16 unk_30;                        /* tile x where the current touch began (MapCursor_UpdateFromTouch); 0xff: none */
    s16 unk_32;                        /* tile y of the same */
    u8 pad_34[0x38 - 0x34];
    MapUnit *actingUnit;               /* &units[actingUnitIndex] */
    u8 actingUnitIndex;                /* slot in units of the unit the AI is moving */
    u8 pad_3d;
    s16 actingUnitStartX;              /* where the acting unit stood before it moved */
    s16 actingUnitStartY;
    s16 actingUnitDestX;               /* where the acting unit's move ends; ActingUnit_CommitMove puts it there */
    s16 actingUnitDestY;
    u8 unk_46;                         /* passed to Unit_GetFuelUsed with actingUnit to get fuel used */
    u8 pad_47;
    u32 unk_48;                        /* cleared with unk_4c, unk_50 and unk_54 by Screen_InitSystems */
    u32 unk_4c;
    u32 unk_50;
    u16 unk_54;
    u8 pad_56[0x58 - 0x56];
    u8 gameMode;                       /* GAME_MODE_*; 2 is likely Versus (4 Force Rank sets per CO, CO_NONE pickable), 3 likely Survival (Player_AddFunds) */
    u8 unk_59;
    u8 pad_5a;
    u8 unk_5b;                         /* bit 1 set by func_020c0704 (map screen setup and Players_Reset) */
    u8 unk_5c;
    u8 coEffectsOn;                    /* name is a guess; 0: no CO luck (good luck 10, bad luck 0) */
    u8 partnerBoostHuman;              /* nonzero: non-AI players get tag partner skills */
    u8 partnerBoostAi;                 /* nonzero: AI players get tag partner skills */
    u8 pad_60[0x64 - 0x60];
    u32 unk_64;
    u32 unk_68;
    u32 unk_6c;
    u32 unk_70;
    u32 unk_74;
    s32 fundsPerProperty;              /* income from each city, base, airport, port and HQ */
    u32 unk_7c;
    u8 unk_80;                         /* from MapHeader.unk_30 */
    u8 unk_81;                         /* from MapHeader.unk_34 */
    u8 unk_82;
    u8 musicReady;                     /* 0: song requests are kept pending, see Sound_PlaySong; name a guess */
    u8 unk_84;
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
    u8 pad_a6[0xaa - 0xa6];
    u16 unk_aa[8];
    u8 coSelectPhase;                  /* 1 on maps where a player picks a CO; turns off Scout and Eagle Eye, see Map_IsCoSelectPhase */
    u8 pad_bb;
    u8 sidePanelRight;                 /* 1: the side panel sits on the right of the screen, 0: on the left (SidePanel_*); name is a guess at what the panel shows */
    u8 pad_bd[0xc0 - 0xbd];
    s32 sidePanelX;                    /* x of the side panel; 0 or 192 at rest, see SidePanel_SlideLeft */
    u8 pad_c4[0xd0 - 0xc4];
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
    Unk_02080440_608c unk_608c[92];    /* tiles with urban terrain (or terrain 16 in game mode 4), filled by func_020c4448; the list ends at tile 0xff when not full */
    Combatant combatants[2];           /* attacker, defender */
} Unk_02080440_48;

/* Reached through Unk_02080440.unk_5c[i]. */
typedef struct Unk_02080440_5c {
    u8 unk_00;                         /* sent to other consoles with the weather (Link_SendWeather) */
    u8 unk_01;                         /* same */
    u8 unk_02;                         /* same */
    u8 weatherDays[5];                 /* per player slot: days the current weather lasts; set by Weather_SetDays */
} Unk_02080440_5c;

/* Player slot that owns a unit in state->units: slot 0 is neutral, so the
 * first MAP_UNITS_PER_PLAYER (64) units belong to slot 1. */
#define MAP_UNIT_OWNER(state, unit) ((((unit) - (state)->units) >> 6) + 1)

/* One of the 100-byte records in data_02169f48 that Screens_SelectConfigs
 * gives each game state (Unk_02080440.unk_50). */
typedef struct Unk_02080440_50 {
    void (*first)(void);               /* run by func_020c8888 when set */
    void (*second)(void);              /* run by func_020c8888 after first */
    u8 pad_08[0x18 - 0x08];
    void (*callback)(void);            /* run by func_020f53e8 when set; name is a guess */
    u8 pad_1c[0x38 - 0x1c];
    void (*unk_38)(void);              /* run by func_020f77bc when set */
    u8 pad_3c[0x40 - 0x3c];
    void (*afterBoxDraw)(void);        /* run by MenuBox_Draw after it draws a menu box */
    u8 pad_44[0x48 - 0x44];
    void (*unk_48)(void);              /* run by ScreenConfig_RunActionHook when set */
    void (*unk_4c)(void);              /* run by Script_StartWithHook when unk_48 is set */
    u8 pad_50[0x54 - 0x50];
    void (*unk_54)(int);               /* called with 1 by Battle_SetupWithPresetCos when set */
    u8 pad_58[0x5c - 0x58];
    BOOL (*unk_5c)(void);              /* func_020fa6f8 calls it; picks between two asset or chart variants */
    u8 pad_60[100 - 0x60];
} Unk_02080440_50;

/* Pointed to by data_027c027c (DTCM). */
typedef struct Unk_02080440 {
    u8 pad_00[0x2c];
    struct Task *taskPool;             /* TASK_COUNT tasks */
    struct Task **taskFreeStack;       /* TASK_COUNT + 1 entries, ends with NULL */
    struct Task **taskFreeTop;         /* next free task in taskFreeStack */
    struct Task **taskLists;           /* TASK_LEVELS lists of top-level tasks */
    struct ScriptSlot *scriptSlots;    /* 30 slots, see game/script_slot.h */
    u16 *unk_40;                       /* ScriptSlot_ClearAll sets the u16 it points to to 0 */
    s16 *scriptSlotCurrent;            /* the u16 holds the slot number being started; ScriptSlot_StartInSlot saves it and restores it afterwards (name from that use) */
    Unk_02080440_48 *unk_48[2];        /* indexed by data_027c0284 */
    Unk_02080440_50 *unk_50[2];        /* indexed by data_027c0284; set by Screens_SelectConfigs */
    u8 *unk_58;                        /* 2 flags: show slot n is in use (UnitShowTask_Create's helper looks for a free one) */
    Unk_02080440_5c *unk_5c[1];       /* length unknown, indexed by data_027c0284 like unk_48 */
} Unk_02080440;

#endif
