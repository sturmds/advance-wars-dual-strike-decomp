#ifndef GAME_MAP_SELECT_H
#define GAME_MAP_SELECT_H

/* Shorthands for the map select menu (MapSelect_Draw and its helpers).
 * Each one expands to the full expression on purpose: the original
 * repeats the loads and the MapSelect_GetTab calls every time. */

#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"

int MapSelect_GetTab(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern MenuState gMenuState;
extern MapListTables data_0229c768;

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: through the cast the compiler keeps field offsets out of
 * the literal pool, as the original does. */
#define MENU ((MenuState*)&gMenuState)
#define TABLES data_0229c768
/* Current tab of the menu, 0-3; each tab has its own map list. */
#define TAB() MapSelect_GetTab()
#define LIST_OF_TAB(tab) (TABLES.listOfTab[tab])
#define LIST(tab) (MENU->setup.lists[LIST_OF_TAB(tab)])

/* Map list layout: COLS by ROWS maps per page. */
#define COLS \
  (STATE->gameMode == GAME_MODE_WAR_ROOM ? 7 : (STATE->twoMaps == 0 ? 9 : 8))
#define ROWS (STATE->gameMode == GAME_MODE_WAR_ROOM ? 1 : 3)
/* Page of the selected entry; the map's index is page * ROWS * COLS plus its
 * position on the page. */
#define PAGE() (TABLES.unk_0d0[TAB()][LIST(TAB()).unk_64])
#define SELECTED_MAP_ID()                                                 \
  (LIST(TAB()).mapIds[TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()] + \
                      PAGE() * (ROWS * COLS)])

#endif
