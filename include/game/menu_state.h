#ifndef GAME_MENU_STATE_H
#define GAME_MENU_STATE_H

#include "types.h"
#include "game/campaign_map.h"

/* One list of map ids in the map select menu. */
typedef struct MapList {
    u16 mapIds[50];                    /* length a guess */
    u8 unk_64;                         /* 0-12, indexes the rows of MapListTables */
    u8 mapCount;                       /* maps in the list; MapSelect pages = (mapCount - 1) / (rows * columns) + 1 */
} MapList;

/* Link screen layout of MenuState (LinkPhase_InitScreen). */
typedef struct LinkScreenState {
    u8 unk_020[4];
    u8 armyColours[4];                 /* army colour of each player, from the map header */
    u8 unk_028[4];
    u8 playerCount;                    /* number of players on the map */
} LinkScreenState;

/* Map select and battle setup layout of MenuState. Arrays of 4 from
 * 0x2b0 are indexed by player (players 1-4 at 0-3); [2] is front and back
 * CO, or main and second map for controllers. */
typedef struct BattleSetupState {
    u16 colours2[16];                  /* map select: like MenuState.colours, for a second palette entry */
    u8 pad_040[0x6a - 0x40];
    MapList lists[5];                  /* count a guess; picked by MapListTables.listOfTab */
    u8 pad_268[0x2b0 - 0x268];
    u8 controllers[4][2];              /* PLAYER_CONTROLLER_HUMAN or _AI, on the main and second map */
    u8 cos[4][2];                      /* front and back CO id */
    u8 forceRankSets[4][2];            /* 0-3, from func_02017abc */
    u8 teams[4];                       /* team of each player slot, 0-3 */
    u8 armyColours[4];
    u8 pad_2d0[0x2d4 - 0x2d0];
    u8 *coList;                        /* COs to pick from, filled by BuildCoPickList */
    u16 fundsPerProperty;
    u8 coListCount;
    u8 fog;                            /* 1: Fog of War */
    u8 unk_2dc;                        /* weather setting, 0-4 */
    u8 unk_2dd;                        /* map tileset, bit 7 cleared */
    u8 unk_2de;                        /* Unk_02080440_48.unk_80 */
    u8 unk_2df;                        /* Unk_02080440_48.unk_81 */
    u8 coEffectsOn;                    /* Unk_02080440_48.coEffectsOn */
    u8 unk_2e1;
    u8 unk_2e2;
    u8 unk_2e3;                        /* number of player slots */
    u8 unk_2e4;
    u8 pad_2e5[0x2e6 - 0x2e5];
    u8 unk_2e6;
    u8 unk_2e7;                        /* filled by func_020c47d0 */
    u8 unk_2e8;                        /* filled by func_020c47d0 */
    u8 settingsChanged;            /* set by the first change of a setting on the Vs setup screen, which also plays a sound */
    u8 unk_2ea;
    u8 unk_2eb;                        /* nonzero: take the settings from the players of a running game */
    u8 pad_2ec[0x4cc - 0x2ec];
    u8 mapListCount;                   /* entries MapSelect_DrawListThumbnail cycles through, a guess */
    u8 pad_4cd;
    s8 unk_4ce[6];                     /* drawn as two-digit numbers in a 3 by 2 grid */
    s8 unk_4d4[6];                     /* same, for a second grid */
    u8 recordCos[6];                   /* War Room: front and back CO of each of the three best records */
    u8 recordUnk_0a[3];                /* WarRoomRecord.unk_0a of each record */
    u8 pad_4e3;
    u16 recordUnk_14[3];               /* WarRoomRecord.unk_14 of each record */
    u8 pad_4ea[0x4fc - 0x4ea];
    u16 unk_4fc;
    u8 pad_4fe;
    u8 slideFrames;                    /* frames left of the slide between lists; 15 on the first */
    u8 slideBack;                      /* nonzero: the slide runs the other way */
    u8 subSlideFrames;                 /* frames left of a second slide, of the strip at MapListTables.unk_108 */
    u8 subSlideBack;                   /* nonzero: the second slide runs the other way */
    u8 prevList;                       /* index into lists of the list sliding out */
} BattleSetupState;

/* Campaign world map layout of MenuState (CampaignMap_Init). */
typedef struct CampaignMapWork {
    u8 unk_020[0xc4];                  /* the three blocks are copied from res_gmap */
    u8 unk_0e4[0x160];
    u8 unk_244[0x8c];
    u8 pad_2d0[0x2d8 - 0x2d0];
    CampaignTopCo topCos[3];
    u8 enemyCos[2];                    /* front and back CO of the map's last player slot, probably the enemy */
} CampaignMapWork;

/* gMenuState (gMenuState): work area of the menu screens. The first
 * 0x20 bytes mean the same on every screen; each screen lays out the rest
 * its own way. Field names with an offset give it from the start of
 * MenuState. The symbols data_0229eb70, data_0229ed30 and data_0229ef30
 * point inside it, at 0x40, 0x200 and 0x400. */
/* Layout of MenuState read by func_0206d67c, which draws a unit's
 * firepower (through Player_GetShownFirepower). */
typedef struct Unk_0206d67c {
    u8 pad_020[0xa1 - 0x20];
    u8 player;                         /* passed to Player_GetShownFirepower */
    u8 slot;                           /* passed to Player_GetShownFirepower; 0: front CO */
    u8 pad_0a3[0xa8 - 0xa3];
    u32 unk_0a8;                       /* 1 with gameMode 3: Player_GetShownFirepower uses Player_GetSlotFirepower for every slot */
} Unk_0206d67c;

/* Battle animation layout of MenuState (ov002, Battle_ShowQuote). */
typedef struct BattleScreenWork {
    u8 pad_020[0x490 - 0x20];
    u8 unk_490[0x80];                  /* copied from res_battle 0x828 */
    u8 unk_510[0x240];                 /* copied from res_battle 0x88c */
    u8 unk_750[0x80];                  /* copied from res_battle 0xb54 */
    u8 unk_7d0[0x800];                 /* cleared, then handed to func_ov002_0235e58c */
    u16 unk_fd0[16];                   /* palette picked by data_0229fb30 */
    s32 hpShown[2];                    /* 16.16 fixed point, per side */
    s32 hpStep[2];                     /* 16.16 fixed point, taken off hpShown each frame */
    u8 unk_1000[2];
    u8 pad_1002[2];
    u8 hpStart[2];
    u8 mood[2];                        /* 0 none, 1 happy quote, 2 angry quote */
    u16 unk_1008[2];
    u16 hpFrames[2];                   /* frames the HP drain takes; 0: no drain */
} BattleScreenWork;

typedef struct MenuState {
    u16 colours[16];                   /* a 16-colour palette: map select copies one entry to the
                                        * palette every 4 frames; the link screen and the world map
                                        * load it from a file */
    union {
        LinkScreenState link;
        BattleSetupState setup;
        CampaignMapWork campaignMap;
        Unk_0206d67c unk_0206d67c;
        BattleScreenWork battle;
    };
} MenuState;

/* data_0229ed30, gMenuState + 0x200. BattleSetup_LoadFromGame stores
 * setup.fundsPerProperty through this symbol: naming gMenuState there
 * changes which loads the compiler repeats. */
typedef struct MenuStateFrom200 {
    u8 pad_200[0x2d8 - 0x200];
    u16 fundsPerProperty;              /* BattleSetupState.fundsPerProperty */
} MenuStateFrom200;

/* data_0229ef30, gMenuState + 0x400. MapSelect_GetTab reads
 * setup.unk_4fc through this symbol. */
typedef struct MenuStateFrom400 {
    u8 pad_400[0x4fc - 0x400];
    u16 unk_4fc;                       /* BattleSetupState.unk_4fc */
} MenuStateFrom400;

/* Tables per map select tab (0-3, see MapSelect_GetTab), data_0229c768. */
typedef struct MapListTables {
    u8 unk_000[4][13][4];              /* position of an entry's map on its page, by tab, entry (MapList.unk_64) and page */
    u8 unk_0d0[4][13];                 /* page of an entry, by tab and entry */
    u8 listOfTab[4];                   /* index into BattleSetupState.lists */
    u8 unk_108;
} MapListTables;

#endif
