#ifndef GAME_MENU_STATE_H
#define GAME_MENU_STATE_H

#include "types.h"
#include "game/campaign_map.h"

/* One list of map ids in the map select menu. */
typedef struct MapList {
    u16 mapIds[50];                    /* length a guess */
    u8 unk_64;                         /* 0-12, indexes the rows of MapListTables */
    u8 pad_65;
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
    u8 pad_2e9[0x2ea - 0x2e9];
    u8 unk_2ea;
    u8 unk_2eb;                        /* nonzero: take the settings from the players of a running game */
    u8 pad_2ec[0x4ce - 0x2ec];
    s8 unk_4ce[6];                     /* drawn as two-digit numbers in a 3 by 2 grid */
    s8 unk_4d4[6];                     /* same, for a second grid */
    u8 pad_4da[0x4fc - 0x4da];
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
typedef struct MenuState {
    u16 colours[16];                   /* a 16-colour palette: map select copies one entry to the
                                        * palette every 4 frames; the link screen and the world map
                                        * load it from a file */
    union {
        LinkScreenState link;
        BattleSetupState setup;
        CampaignMapWork campaignMap;
    };
} MenuState;

/* data_0229ed30, gMenuState + 0x200. BattleSetup_LoadFromGame stores
 * setup.fundsPerProperty through this symbol: naming gMenuState there
 * changes which loads the compiler repeats. */
typedef struct MenuStateFrom200 {
    u8 pad_200[0x2d8 - 0x200];
    u16 fundsPerProperty;              /* BattleSetupState.fundsPerProperty */
} MenuStateFrom200;

/* Tables per map select tab (0-3, see MapSelect_GetTab), data_0229c768. */
typedef struct MapListTables {
    u8 unk_000[4][13][4];              /* position of an entry's map on its page, by tab, entry (MapList.unk_64) and page */
    u8 unk_0d0[4][13];                 /* page of an entry, by tab and entry */
    u8 listOfTab[4];                   /* index into BattleSetupState.lists */
    u8 unk_108;
} MapListTables;

#endif
