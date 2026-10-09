#ifndef GAME_MAIN_MENU_H
#define GAME_MAIN_MENU_H

#include "types.h"

/* The main game menu: pages of entries; L and R (or left and right) flip
 * between page 0 and page 6. */

/* One entry of a main menu page. */
typedef struct MainMenuEntry {
    u8 unk_00;
    u8 unk_01 : 7;
    u8 hidden : 1;                     /* the cursor skips it */
} MainMenuEntry;

/* Layout of the current main menu page, through data_0229c71c. */
typedef struct MainMenuPage {
    u8 unk_00;
    u8 unk_01 : 5;
    u8 entryCount : 3;
    MainMenuEntry entries[7];          /* count a guess: entryCount holds up to 7 */
} MainMenuPage;

/* Main menu state, through data_021599d0. */
typedef struct MainMenu {
    u8 cursors[14];                    /* cursor of each page */
    u8 page : 4;
    u8 prevPage : 4;
} MainMenu;

extern MainMenuPage **data_0229c71c;
extern MainMenu *data_021599d0;

#endif
