#ifndef GAME_UNK_OV005_0236BBB0_H
#define GAME_UNK_OV005_0236BBB0_H

#include "types.h"

/* One item for sale, 28 bytes, as in the table at data_0216cb60. */
typedef struct ShopItem {
    int iconId;
    u32 textId;                        /* name */
    int price;
    u8 pad_0c[0x1c - 0x0c];
} ShopItem;

/* Overlay 5 work block (0x1d68 bytes) that func_ov005_02366eec allocates
 * into Ov005Task.work and data_ov005_0236bbb0: Hachi's shop, a list of
 * items with prices (from the 28-byte records at data_0216cb60), six rows
 * per page. */
typedef struct Unk_ov005_0236bbb0 {
    int points;                        /* shown at the top (Ov005_DrawShopPoints); func_ov005_02367ad8 copies the first word of gPlayRecords */
    int unk_04;
    u32 unk_08;
    int unk_0c;
    int unk_10;
    int unk_14;
    int redrawRows;                    /* 1: Ov005_DrawShopPage clears the rows first */
    int unk_1c;
    u32 shopItemCount;                 /* records of data_0216cb60 that func_ov005_02367ad8 scans */
    int cursorRow;                     /* row on the page, 0-5 */
    u8 pad_28[0x34 - 0x28];
    int page;                          /* six items per page */
    u8 pad_38[0x108 - 0x38];
    int itemCount;                     /* items in `items` */
    int highlightCursorRow;                       /* 1: the cursor row's name and price use palette 9 */
    int showCursor;                       /* 1: the menu cursor shows on the cursor row */
    ShopItem *items;                   /* the items for sale, filled by func_ov005_02367ad8 */
    u8 pad_118[0x1d64 - 0x118];
    struct Task *songTask;             /* func_020fcbd8(12, 0): fades to song 12 */
} Unk_ov005_0236bbb0;

#endif
