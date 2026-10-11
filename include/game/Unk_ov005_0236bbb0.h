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
    const struct EventScriptCommand *confirmScript; /* asks whether to buy (Ov005_UpdateShopScreen) */
} ShopItem;

/* One of the six buttons of a page, 12 bytes. */
typedef struct ShopButton {
    s16 shown;                         /* nonzero: the button exists */
    s16 buttonId;                      /* MenuButton id */
    s16 x;
    s16 y;
    u8 pad_08[0xc - 0x8];
} ShopButton;

/* Overlay 5 work block (0x1d68 bytes) that Ov005_AllocShopWork allocates
 * into Ov005Task.work and data_ov005_0236bbb0: Hachi's shop, a list of
 * items with prices (from the 28-byte records at data_0216cb60), six rows
 * per page. */
typedef struct Unk_ov005_0236bbb0 {
    int points;                        /* shown at the top (Ov005_DrawShopPoints); Ov005_LoadShopItems copies gPlayRecords.points */
    int buyRequested;                  /* set by a row's touch button (Ov005_RequestShopBuy); acts like A */
    u32 leaveRequested;                /* set by a touch button; acts like B */
    int prevPageRequested;             /* set by a touch button; acts like L */
    int nextPageRequested;             /* set by a touch button; acts like R */
    int transition;                    /* 1: EventPicture_DrawAtStep level eases 6 to 0, 2: 0 to 6 (Ov005_UpdateShopTransition); 0 when done */
    int redrawRows;                    /* 1: Ov005_DrawShopPage clears the rows first */
    int boughtSomething;               /* 1 once an item was bought; changes how the shop exits (steps 100-103) */
    u32 shopItemCount;                 /* records of data_0216cb60 that Ov005_LoadShopItems scans */
    int cursorRow;                     /* row on the page, 0-5 */
    int drawnCursorRow;                /* cursorRow of the last frame; -1 forces a redraw */
    int savedCursorRow;                /* cursorRow, restored after a page turn */
    int buttonCount;                   /* buttons on the current page */
    int page;                          /* six items per page */
    int lastPage;                      /* (itemCount - 1) / 6 */
    u32 iconCount;                     /* entries of icons in use */
    u32 icons[50];                     /* icon ids loaded so far (Ov005_LoadShopIcon) */
    int itemCount;                     /* items in `items` */
    int highlightCursorRow;            /* 1: the cursor row's name and price use palette 9 */
    int showCursor;                    /* 1: the menu cursor shows on the cursor row */
    ShopItem *items;                   /* the items for sale, filled by Ov005_LoadShopItems */
    ShopItem itemBuf[256];             /* items points here */
    ShopButton *buttons;               /* points at buttonBuf */
    ShopButton buttonBuf[6];
    struct Task *songTask;             /* Sound_StartSongSwitch(12, 0): fades to song 12 */
} Unk_ov005_0236bbb0;

#endif
