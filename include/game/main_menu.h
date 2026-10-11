#ifndef GAME_MAIN_MENU_H
#define GAME_MAIN_MENU_H

#include "types.h"

/* The main game menu: pages of entries; L and R (or left and right) flip
 * between page 0 and page 6. */

/* One entry of a main menu page. */
typedef struct MainMenuEntry {
    u8 kind;                           /* picks the label graphic, help text and confirm action */
    u8 palette : 7;
    u8 hidden : 1;                     /* the cursor skips it */
} MainMenuEntry;

/* Layout of the current main menu page, through data_0229c71c. */
typedef struct MainMenuPage {
    u8 title;                          /* title graphic */
    u8 palette : 5;
    u8 entryCount : 3;
    MainMenuEntry entries[7];          /* count a guess: entryCount holds up to 7 */
} MainMenuPage;

/* Main menu state, through data_021599d0. */
typedef struct MainMenu {
    u8 cursors[14];                    /* cursor of each page */
    u8 page : 4;
    u8 prevPage : 4;
    u8 nextPage : 4;                   /* page to show after the slide */
    u8 unk_0f_4 : 4;
    u8 slide : 3;                      /* 2: slide left, 3: slide right */
    u8 flipFrames : 5;                 /* page flip frames left, from 16 */
    u8 unk_11;                         /* bit 0 cleared by MainMenu_ClearCursors */
} MainMenu;

/* The 60-byte block data_0229c71c points to; MainMenu_Setup allocates it. */
typedef struct MainMenuWork {
    MainMenuPage *page;
    u8 pad_04[0x08 - 0x04];
    u16 unk_08[9];                     /* colour 4 of each of the 9 palettes in res_modemenu */
    u8 pad_1a[0x1c - 0x1a];
    u8 unk_1c[32];                     /* a 16-colour palette from res_modemenu */
} MainMenuWork;

/* The main menu's task: Task fields, with these two read as halfwords. */
typedef struct MainMenuTask {
    u8 pad_00[0x4c];
    s16 unk_4c;
    u8 pad_4e[0x52 - 0x4e];
    u16 unk_52;                        /* frame count & 7 at setup */
} MainMenuTask;

extern MainMenuPage **data_0229c71c;
extern MainMenu *data_021599d0;

#endif
