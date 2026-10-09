#ifndef GAME_UNK_02066818_H
#define GAME_UNK_02066818_H

#include "types.h"

/* One list of map ids in the map select menu. */
typedef struct MapList {
    u16 mapIds[50];                    /* length a guess */
    u8 unk_64;                         /* 0-12, indexes the rows of MapListTables */
    u8 pad_65;
} MapList;

/* data_0229eb30: menu state shared by the pre-battle screens. Arrays of
 * 4 from 0x2b0 are indexed by player (players 1-4 at 0-3); [2] is front and
 * back CO, or main and second map for controllers. */
typedef struct Unk_02066818 {
    /* The link screen (LinkPhase_InitScreen) and the map select menu
     * (MapSelect_Draw) use these bytes differently. */
    union {
        struct {
            u8 unk_000[0x20];
            u8 unk_020[4];
            u8 unk_024[4];    /* army colour of each player, from the map header */
            u8 unk_028[4];
            u8 unk_02c;       /* number of players on the map */
        } link;
        struct {
            u16 colours[16];  /* one copied to the palette every 4 frames */
            u16 colours2[16]; /* same for a second palette entry */
        } mapSelect;
    };
    u8 pad_040[0x6a - 0x40];
    MapList lists[5];                  /* count a guess; picked by MapListTables.listOfTab */
    u8 pad_268[0x2b0 - 0x268];
    u8 controllers[4][2];              /* PLAYER_CONTROLLER_HUMAN or _AI, on the main and second map */
    u8 cos[4][2];                      /* front and back CO id */
    u8 forceRankSets[4][2];            /* 0-3, from func_02017abc */
    u8 unk_2c8[4]; /* team of each player slot, 0-3 */
    u8 armyColours[4];
    u8 pad_2d0[0x2d4 - 0x2d0];
    u8 *coList;                        /* COs to pick from, filled by func_020e3478 */
    u8 pad_2d8[0x2da - 0x2d8];
    u8 coListCount;
    u8 fog;                            /* 1: Fog of War */
    u8 unk_2dc;                        /* weather setting, 0-4 */
    u8 unk_2dd;                        /* map tileset, bit 7 cleared */
    u8 unk_2de;                        /* Unk_02080440_48.unk_80 */
    u8 unk_2df;                        /* Unk_02080440_48.unk_81 */
    u8 coEffectsOn;                    /* Unk_02080440_48.coEffectsOn */
    u8 unk_2e1;
    u8 unk_2e2;
    u8 unk_2e3;       /* number of player slots */
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
} Unk_02066818;

/* Tables per map select tab (0-3, see func_020642e8), data_0229c768. */
typedef struct MapListTables {
    u8 unk_000[4][13][4];              /* position of an entry's map on its page, by tab, entry (MapList.unk_64) and page */
    u8 unk_0d0[4][13];                 /* page of an entry, by tab and entry */
    u8 listOfTab[4];                   /* index into Unk_02066818.lists */
    u8 unk_108;
} MapListTables;

/* data_0229ed30, 0x200 bytes after data_0229eb30. */
typedef struct Unk_0229ed30 {
    u8 pad_00[0xd8];
    u16 fundsPerProperty;
} Unk_0229ed30;

#endif
