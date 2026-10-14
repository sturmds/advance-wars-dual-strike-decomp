#ifndef GAME_MENU_STATE_H
#define GAME_MENU_STATE_H

#include "types.h"
#include "game/player.h"
#include "game/campaign_map.h"
#include "game/BattleSideData.h"
#include "game/battle_layout.h"
#include "game/battle_scene.h"
#include "game/battle_fighter.h"
#include "game/battle_hit_list.h"
#include "game/co_exp.h"
#include "game/design_map.h"
#include "game/Unk_02160ac0.h"

/* One list of map ids in the map select menu. */
typedef struct MapList {
  u16 mapIds[50]; /* length a guess */
  u8 unk_64;      /* 0-12, indexes the rows of MapListTables */
  u8 mapCount; /* maps in the list; MapSelect pages = (mapCount - 1) / (rows * columns) + 1 */
} MapList;

/* All map lists of a game mode, as MapSelect_BuildWarRoomLists (War Room) and
 * MapSelect_BuildLists fill them. Same layout as MenuState from 0x6a to
 * BattleSetupState.mapListCount. */
typedef struct MapListSet {
  MapList lists[11];
  u8 count;
} MapListSet;

/* Link screen layout of MenuState (LinkPhase_InitScreen). */
typedef struct LinkScreenState {
  u8 panelPlayers[4]; /* player shown in each panel */
  u8 armyColours[4];  /* army colour of each player, from the map header */
  u8 readyFlags[4];   /* per player: 1 once ready */
  u8 playerCount;     /* number of players on the map */
  u8 pad_02d[0x162e - 0x2d];
  u8 picked
      [2]; /* per link player: has picked a button of the two-player link menu */
} LinkScreenState;

/* Ready flag of a player on the link screen, which is LinkScreenState.readyFlags[player]. The
 * game adds the player index to the base first, then the offset 0x28; the cast keeps that
 * order, which the match needs. */
#define LINK_READY_FLAG(menuState, player) \
  (((u8*)&(menuState) + (player))[0x28])

/* Player shown in a panel of the link screen, which is LinkScreenState.panelPlayers[panel]; written the same way. */
#define LINK_PANEL_PLAYER(menuState, panel) \
  (((u8*)&(menuState) + (panel))[0x20])

/* Map select and battle setup layout of MenuState. Arrays of 4 from
 * 0x2b0 are indexed by player (players 1-4 at 0-3); [2] is front and back
 * CO, or main and second map for controllers. */
typedef struct BattleSetupState {
  u16 colours2
      [16]; /* map select: like MenuState.colours, for a second palette entry */
  u8 pad_040[0x6a - 0x40];
  MapList lists[5]; /* count a guess; picked by MapListTables.listOfTab */
  u8 pad_268[0x2b0 - 0x268];
  u8 controllers
      [4][2];   /* PLAYER_CONTROLLER_HUMAN or _AI, on the main and second map */
  u8 cos[4][2]; /* front and back CO id */
  u8 forceRankSets[4][2]; /* 0-3, from Co_GetForceRankSetIndex */
  u8 teams[4];            /* team of each player slot, 0-3 */
  u8 armyColours[4];
  u8 ready[4]; /* by player slot: 1 once the slot is ready on the team screen */
  u8* coList;  /* COs to pick from, filled by BuildCoPickList */
  u16 fundsPerProperty;
  u8 coListCount;
  u8 fog;         /* 1: Fog of War */
  u8 unk_2dc;     /* weather setting, 0-4 */
  u8 unk_2dd;     /* map tileset, bit 7 cleared */
  u8 unk_2de;     /* Unk_02080440_48.unk_80 */
  u8 unk_2df;     /* Unk_02080440_48.unk_81 */
  u8 coEffectsOn; /* Unk_02080440_48.coEffectsOn */
  u8 visualMode;  /* VISUAL_* */
  u8 unk_2e2;
  u8 unk_2e3;        /* number of player slots */
  u8 cursorButton;   /* id of the selected setup button; 8-15 are the COs */
  u8 selectedSlot;   /* player slot picked on the team screen */
  u8 settingRow;     /* selected row of the settings menu, 0-8 */
  u8 unk_2e7;        /* filled by Map_LoadAndCountProperties */
  u8 unk_2e8;        /* filled by Map_LoadAndCountProperties */
  u8 cursorSePlayed; /* set when a Vs setup change plays SE 0x10002; VsSetup_UpdateLink clears it each frame, so the sound plays once a frame */
  u8 unk_2ea;
  u8 unk_2eb; /* nonzero: take the settings from the players of a running game */
  u8 pad_2ec[0x4cc - 0x2ec];
  u8 mapListCount; /* entries MapSelect_DrawListThumbnail cycles through, a guess */
  u8 pad_4cd;
  s8 unk_4ce[6]; /* drawn as two-digit numbers in a 3 by 2 grid */
  s8 unk_4d4[6]; /* same, for a second grid */
  u8 recordCos
      [6]; /* War Room: front and back CO of each of the three best records */
  u8 recordDays[3]; /* WarRoomRecord.days of each record */
  u8 pad_4e3;
  u16 recordScores[3]; /* WarRoomRecord.score of each record */
  u8 prevRecordCos
      [6]; /* the records above as they were before the list slide */
  u8 prevRecordDays[3];
  u8 pad_4f3;
  u16 prevRecordScores[3];
  u16 prevMapId;  /* map selected before the list changed */
  u16 firstMapId; /* first map of a two-map pick; 0: none picked yet */
  u8 listChanged; /* set by the first change of list, which plays a sound */
  u8 slideFrames; /* frames left of the slide between lists; 15 on the first */
  u8 slideBack;   /* nonzero: the slide runs the other way */
  u8 subSlideFrames; /* frames left of a second slide, of the strip at MapListTables.subStrip */
  u8 subSlideBack; /* nonzero: the second slide runs the other way */
  u8 prevList;     /* index into lists of the list sliding out */
} BattleSetupState;

/* Campaign world map layout of MenuState (CampaignMap_Init). */
typedef struct CampaignMapWork {
  u8 unk_020[0xc4]; /* the three blocks are copied from res_gmap */
  u8 unk_0e4[0x160];
  u8 unk_244[0x8c];
  struct ShowSprite* sprites[2]; /* made by CampaignMap_InitCrossingSprites */
  CampaignTopCo topCos[CAMPAIGN_TOP_CO_COUNT];
  u8 enemyCos
      [2]; /* front and back CO of the map's last player slot, probably the enemy */
} CampaignMapWork;

/* gMenuState (gMenuState): work area of the menu screens. The first
 * 0x20 bytes mean the same on every screen; each screen lays out the rest
 * its own way. Field names with an offset give it from the start of
 * MenuState. The symbols data_0229eb70, data_0229ed30 and data_0229ef30
 * point inside it, at 0x40, 0x200 and 0x400. */
/* Layout of MenuState for the player info (CO stats) screen, filled by
 * PlayerInfo_Init. */
typedef struct PlayerInfoScreen {
  u8 pad_020[0x8c - 0x20];
  u8 skills
      [SKILL_LIST_LENGTH +
       1]; /* skill ids the shown CO has equipped, ended by 0 or 0xff (CoStatsScreen_DrawSkills) */
  u8 coShown[4][2]; /* nonzero: this CO of the player gets a button */
  u8 playerArmy
      [4]; /* per player 1-4 (index player - 1); guess: army colour, ARMY_BLACK_HOLE (5) as in UnitRow_IsAvailable */
  u8 unk_09d
      [3]; /* per row 0-2: number of icons PlayerInfo_DrawPortraitScreenSprites queues on the row; 0 ends the rows */
  u8 playerCount;
  u8 player;       /* passed to Player_GetShownFirepower */
  u8 slot;         /* passed to Player_GetShownFirepower; 0: front CO */
  u8 page;         /* 0-3; Down (PlayerInfo_NextPage) turns to the next */
  u8 lastPageView; /* 0 or 1, R toggles it (PlayerInfo_ToggleLastPageView); 1 adds two sprites on page 3 */
  u8 pad_0a5[0xa8 - 0xa5];
  u32 unk_0a8; /* 1 with gameMode 3: Player_GetShownFirepower uses Player_GetSlotFirepower for every slot */
} PlayerInfoScreen;

/* Battle animation layout of MenuState (ov002, Battle_ShowQuote). */
struct BattleSpriteTask;

typedef struct BattleEntry {
  s32 unk_00; /* BattleCellTask *; cleared by Battle_ResetScreen */
  struct BattleSpriteTask*
      sprites[4]; /* sprites the cell task moves; 0 when not made */
  u8 fillBefore;  /* Battle_CompareHpToCellStep of the HP before the fight */
  u8 fillAfter;   /* the same for the HP after it */
  u8 fillShown;   /* starts as fillBefore */
  u8 pad_17;
} BattleEntry;

/* Offset of the 3D scene on one screen. */
typedef struct BattleSway {
  s16 x;
  s16 y;
  s16 z;
  u16 angle; /* (angle + 0x600) >> 4 picks a sine and cosine pair in data_0214d4c4 */
} BattleSway;

struct BattleSideData;

/* A sprite on one cell of a side's HP bar, 5 per side. */
struct BattleSpriteTask;

typedef struct BattleCellSprite {
  struct BattleSpriteTask* task;
  BattlePoint pos;  /* start position */
  BattlePoint step; /* added to pos each frame */
  BattlePoint direction;
  BattlePoint target;
  u8 cell : 3;
  u8 unk_14_3 : 1;
  u8 unk_14_hi : 4;
  u8 pad_15[3];
} BattleCellSprite;

typedef struct BattleScreenWork {
  u16 unk_20[BATTLE_SIDE_COUNT]; /* per side */
  u16 unk_24; /* palette entry of the extra sprite of BattleCellTask_RemakeSpritesWithEffect */
  u16 pipeTexture; /* texture drawn under a Piperunner, in 32-byte units of texture VRAM */
  const u32* spriteTables
      [BATTLE_SIDE_COUNT]; /* table argument of BattleSpriteTask_Create for the HP cells, per side */
  const u32* spriteTables2
      [BATTLE_SIDE_COUNT]; /* per side: the HP cells' second sprite, from BattleSideData.unk_08 */
  const u32* spriteTables3
      [BATTLE_SIDE_COUNT]; /* per side: the third sprite, from BattleSideData.unk_10 */
  void* unk_40
      [BATTLE_SIDE_COUNT]; /* per side; nonzero once the side's animation is up, a guess */
  u32 unk_48
      [BATTLE_SIDE_COUNT]; /* tables BattleSpriteTask_Create makes the sprites of a side from */
  const u32* hitSpriteTables
      [BATTLE_SIDE_COUNT]; /* per side: sprite made on a hit of the second hit list, a guess */
  const u32* unk_58[BATTLE_SIDE_COUNT];  /* per side, like spriteTables */
  const u32* unk_60[BATTLE_SIDE_COUNT];  /* per side, like spriteTables */
  const void* unk_68[BATTLE_SIDE_COUNT]; /* per side */
  const u32* unk_70; /* sprite table, the same for both sides */
  BattleMotion motions[BATTLE_SIDE_COUNT]
                      [BATTLE_CELL_COUNT]; /* by side, then by cell */
  union {
    BattleEntry unk_13c
        [BATTLE_SIDE_COUNT]
        [BATTLE_CELL_COUNT]; /* only unk_00 is cleared (Battle_ClearMotions) */
    BattleCellSprite cellSprites[BATTLE_SIDE_COUNT][BATTLE_CELL_COUNT];
  };
  BattleCounterTask* unk_22c[BATTLE_SIDE_COUNT]
                            [BATTLE_CELL_COUNT]; /* per side */
  BattleCounterTask* unk_254[BATTLE_SIDE_COUNT]
                            [BATTLE_CELL_COUNT]; /* per side */
  struct BattleSideData* unk_27c[BATTLE_SIDE_COUNT];
  BattleSway sway[BATTLE_SIDE_COUNT]; /* eased to 0 by Battle_UpdateSway */
  u16 unk_294[BATTLE_SIDE_COUNT];     /* per side, like unk_29c */
  u16 unk_298[BATTLE_SIDE_COUNT];     /* per side, like unk_29c */
  u16 unk_29c
      [BATTLE_SIDE_COUNT]; /* per side: five 3-bit entry numbers, the first in the low bits */
  u16 unk_2a0[BATTLE_SIDE_COUNT]; /* per side, like unk_29c */
  BattleHitList
      hitLists[BATTLE_SIDE_COUNT]; /* per side, built by Battle_BuildHitLists */
  BattleHitList stepLists
      [BATTLE_SIDE_COUNT]; /* per side, built by func_ov002_023552d0 from the other side's hitLists */
  BattleHitList unk_2ec
      [BATTLE_SIDE_COUNT]; /* per side, built by func_ov002_023549c8 from stepLists */
  BattleEntry unk_310[BATTLE_SIDE_COUNT][BATTLE_CELL_COUNT];
  const void* drainProfile
      [BATTLE_SIDE_COUNT]; /* gBattleDrainProfile*, set by Battle_PickHpDrainProfiles */
  u16 bgScroll
      [BATTLE_SIDE_COUNT]; /* added to the BG2/BG3 x scroll by Battle_DrawScene; cleared by Battle_Init */
  u16 unk_40c
      [BATTLE_SIDE_COUNT]; /* cleared by Battle_Init; Battle_UpdateSway sets them to sway[0].x >> 4 */
  u16 unk_410[32];   /* unk_450 brightened halfway to white */
  u16 unk_450[32];   /* a side's palette, loaded by File_ReadTo */
  u8 unk_490[0x80];  /* copied from res_battle 0x828 */
  u8 unk_510[0x240]; /* copied from res_battle 0x88c */
  u8 unk_750[0x80];  /* copied from res_battle 0xb54 */
  u8 unk_7d0[0x800]; /* cleared, then handed to Battle_BuildSideTilemaps */
  u16 unk_fd0[16];   /* palette picked by data_0229fb30 */
  s32 hpShown[BATTLE_SIDE_COUNT]; /* 16.16 fixed point, per side */
  s32 hpStep
      [BATTLE_SIDE_COUNT]; /* 16.16 fixed point, taken off hpShown each frame */
  u8 unk_1000[BATTLE_SIDE_COUNT];
  u8 pad_1002[2];
  u8 hpStart[BATTLE_SIDE_COUNT];
  u8 mood[BATTLE_SIDE_COUNT]; /* 0 none, 1 happy quote, 2 angry quote */
  u16 unk_1008[BATTLE_SIDE_COUNT];
  u16 hpFrames[BATTLE_SIDE_COUNT]; /* frames the HP drain takes; 0: no drain */
  u32 frames; /* Unk_0229fb30.unk_10: battle screen frame counter */
} BattleScreenWork;

/* One team row of the Combat setup screens (link play), 10 bytes. */
typedef struct CombatRowEntry {
  union {
    struct {
      u8 player; /* the data_0229fb30.unk_45a this row belongs to */
      u8 pad_01[6];
    };
    u8 members
        [7]; /* link players on the row, the first one picks for it; memberCount of them are used */
  };
  u8 memberCount; /* players on the row; it also needs that many units before it can be marked ready; nonzero: the row gets a sprite on the sub screen */
  u8 locked; /* 1 once the row is marked ready (CombatSetup_MarkReady, CombatSetup_LockRow): no more buying or CO changes */
  u8 buttonId; /* MenuButton id picked, 22-25; minus 22 picks the row's unit counts and sprite */
} CombatRowEntry;

/* Combat setup layout of MenuState (link play, overlay 3's rows). */
typedef struct CombatMenuState {
  u8 pad_020[0x600 - 0x20];
  u16 unk_600
      [0x300]; /* a second tile map from res_cmlink, palette numbers raised by 2 */
  u16 savedMap[0x300]; /* copied to layer 1 of the main screen */
  u16 rowPalettes[COMBAT_ROW_COUNT]
                 [48]; /* per row, loaded at palette buffer offset 0x2c0 */
  u16 rowObjPalettes[COMBAT_ROW_COUNT]
                    [16]; /* per row, loaded at palette buffer offset 0x320 */
  u16 unk_1400[16];       /* a palette from res_linkcommon */
  CombatRowEntry rows[COMBAT_ROW_COUNT];
  struct {
    u8 row;   /* 1-4, index + 1 into rows */
    u8 ready; /* 1 while the player has marked ready (CombatSetup_PlayerReady) */
  } players[8]; /* by link player, bits of Unk_0229ff30.playerMask */
} CombatMenuState;

/* Layout of MenuState read by SpriteStrip_Queue: the 16 shared halfwords at the start
 * (MenuState.colours) are the x positions of the sprites. */
typedef struct SpriteStripState {
  u8 pad_020[0x20];
  u8 spriteCount; /* entries of MenuState.colours that are drawn */
} SpriteStripState;

/* Players of the two-player link menu. */
#define LINK_MENU_PLAYER_COUNT 2

/* Two-player link menu layout of MenuState. */
typedef struct LinkMenuMaps {
  DesignMapRecord maps[LINK_DESIGN_MAPS_PER_PLAYER]; /* MenuState.linkMaps */
  DesignMap rows
      [LINK_MENU_PLAYER_COUNT *
       LINK_DESIGN_MAPS_PER_PLAYER]; /* LINK_MAP_ROWS: 3 per link player, received from each player */
} LinkMenuMaps;

/* Bytes in gMenuState. */
#define MENU_STATE_SIZE 0x8000

typedef struct MenuState {
  u16 colours[16]; /* a 16-colour palette: map select copies one entry to the
                                        * palette every 4 frames; the link screen and the world map
                                        * load it from a file */
  union {
    LinkScreenState link;
    BattleSetupState setup;
    CampaignMapWork campaignMap;
    PlayerInfoScreen unk_0206d67c;
    SpriteStripState spriteStrip;
    BattleScreenWork battle;
    CoExpScreenWork coExp;
    CombatMenuState combat;
    DesignMapRecord linkMaps
        [LINK_DESIGN_MAPS_PER_PLAYER]; /* the maps of the two-player link menu, shown by its stage 2 buttons */
    LinkMenuMaps linkMenu;
    /* the whole work area: ov003 keeps two CO pictures in it, from offset 0 */
    u8 work[MENU_STATE_SIZE - 16 * sizeof(u16)];
  };
} MenuState;

/* BattleScreenWork.unk_27c[side]. Needed to match: the compiler adds the 0x27c after
 * the side index, so the array is reached through gMenuState's own address. */
#define BATTLE_SIDE_DATA_AT(side) \
  (((BattleSideData**)&gMenuState + (side))[0x27c / 4])

/* MenuState.linkMaps. Needed to match: the compiler adds the 0x20 at run time when the
 * array is reached through a cast of gMenuState's address. */
#define LINK_MAPS \
  ((DesignMapRecord*)((u8*)&gMenuState + sizeof(gMenuState.colours)))

/* Summary rows of the maps in the two-player link menu: 3 per link player, behind
 * MenuState.linkMaps (DesignMap.playerCount 0xff: no map in the slot). */
#define LINK_MAP_ROWS                                           \
  ((DesignMap*)((u8*)&gMenuState + sizeof(gMenuState.colours) + \
                sizeof(DesignMapRecord) * LINK_DESIGN_MAPS_PER_PLAYER))

/* Offset of LinkScreenState.picked in MenuState. Needed to match: the compiler adds it after
 * the player index, so the flags are reached through gMenuState's own address plus the player. */
#define LINK_PICKED_OFFSET 0x162e

/* data_0229ed30, gMenuState + 0x200. BattleSetup_LoadFromGame stores
 * setup.fundsPerProperty through this symbol: naming gMenuState there
 * changes which loads the compiler repeats. */
typedef struct MenuStateFrom200 {
  u8 pad_200[0x284 - 0x200];
  BattleSway sway[2]; /* BattleScreenWork.sway */
  u8 pad_294[0x2d8 - 0x294];
  u16 fundsPerProperty; /* BattleSetupState.fundsPerProperty */
} MenuStateFrom200;

/* data_0229ef30, gMenuState + 0x400. MapSelect_GetTab reads
 * setup.firstMapId through this symbol. */
typedef struct MenuStateFrom400 {
  u8 pad_400[0x408 - 0x400];
  u16 bgScroll[2]; /* BattleScreenWork.bgScroll */
  u16 unk_40c[2];  /* BattleScreenWork.unk_40c */
  u8 pad_410[0x4fa - 0x410];
  u16 prevMapId;  /* BattleSetupState.prevMapId */
  u16 firstMapId; /* BattleSetupState.firstMapId */
} MenuStateFrom400;

/* gMenuState on the result screen (ResultScreen_Init). The blocks
 * are copied from /ohashi/res_result and res_result_lang; it starts at
 * offset 0, over MenuState.colours. */
typedef struct ResultScreenWork {
  u8 unk_000[0x16c]; /* 0x16a bytes from res_result 0x59d8 */
  u8 unk_16c[0xac];  /* 0xaa bytes from res_result 0x5b44 */
  u8 unk_218[0x4c];  /* 0x4a bytes from res_result 0x5bf0 */
  u8 unk_264[1];     /* res_result_lang unpacked; size unknown */
} ResultScreenWork;

#define RESULT_WORK ((ResultScreenWork*)&gMenuState)

/* Tables per map select tab (0-3, see MapSelect_GetTab), data_0229c768. */
typedef struct MapListTables {
  u8 unk_000
      [4][13]
      [4]; /* position of an entry's map on its page, by tab, entry (MapList.unk_64) and page */
  u8 unk_0d0[4][13]; /* page of an entry, by tab and entry */
  u8 listOfTab[4];   /* index into BattleSetupState.lists */
  u8 subStrip; /* War Room strip 0-3 shown under the list (MapSelect_DrawSubStrip); indexes gWarRoomRecords */
} MapListTables;

#endif
