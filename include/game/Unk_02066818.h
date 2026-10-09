#ifndef GAME_UNK_02066818_H
#define GAME_UNK_02066818_H

#include "types.h"

/* One list of map ids in the map select menu. */
typedef struct MapList {
    u16 mapIds[50];                    /* length a guess */
    u8 unk_64;                         /* 0-12, indexes the rows of MapListTables */
    u8 pad_65;
} MapList;

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
    u8 pad_268[0x2c8 - 0x268];
    u8 unk_2c8[0x1b]; /* team of each player slot, 0-3 */
    u8 unk_2e3;       /* number of player slots */
    u8 pad_2e4[0x4ce - 0x2e4];
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

#endif
