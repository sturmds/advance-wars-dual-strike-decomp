#ifndef GAME_UNK_OV005_0236BBB0_H
#define GAME_UNK_OV005_0236BBB0_H

#include "types.h"

/* One item for sale, 28 bytes, as in the table at data_0216cb60. */
typedef struct ShopItem {
    int iconId;
    u32 textId;                        /* name */
    int price;                         /* -1 ends the table */
    int (*isForSale)(int arg);         /* 1: the item is offered */
    void (*onBuy)(int arg, int flag);  /* run when the item is bought */
    int arg;                           /* passed to isForSale and onBuy */
    u8 pad_18[0x1c - 0x18];
} ShopItem;

/* One of the six buttons of a page, 12 bytes. */
typedef struct ShopButton {
    s16 shown;                         /* nonzero: the button exists */
    s16 buttonId;                      /* MenuButton id */
    u8 pad_04[0xc - 0x4];
} ShopButton;

/* Overlay 5 work block (0x1d68 bytes) that Ov005_AllocShopWork allocates
 * into Ov005Task.work and data_ov005_0236bbb0: Hachi's shop, a list of
 * items with prices (from the 28-byte records at data_0216cb60), six rows
 * per page. */
typedef struct Unk_ov005_0236bbb0 {
    int points;                        /* shown at the top (Ov005_DrawShopPoints); Ov005_LoadShopItems copies the first word of gPlayRecords */
    int unk_04;
    u32 unk_08;
    int unk_0c;
    int unk_10;
    int unk_14;
    int redrawRows;                    /* 1: Ov005_DrawShopPage clears the rows first */
    int unk_1c;
    u32 shopItemCount;                 /* records of data_0216cb60 that Ov005_LoadShopItems scans */
    int cursorRow;                     /* row on the page, 0-5 */
    int unk_28;
    int unk_2c;                        /* set to cursorRow when it changes */
    int buttonCount;                   /* buttons on the current page */
    int page;                          /* six items per page */
    int pageCount;
    u32 iconCount;                     /* entries of icons in use */
    u32 icons[50];                     /* icon ids loaded so far (Ov005_LoadShopIcon) */
    int itemCount;                     /* items in `items` */
    int highlightCursorRow;                       /* 1: the cursor row's name and price use palette 9 */
    int showCursor;                       /* 1: the menu cursor shows on the cursor row */
    ShopItem *items;                   /* the items for sale, filled by Ov005_LoadShopItems */
    ShopItem itemBuf[256];             /* items points here */
    ShopButton *buttons;               /* points at buttonBuf */
    ShopButton buttonBuf[6];
    struct Task *songTask;             /* func_020fcbd8(12, 0): fades to song 12 */
} Unk_ov005_0236bbb0;

#endif
