#ifndef GAME_MAP_SELECT_H
#define GAME_MAP_SELECT_H

/* Shorthands for the map select menu (MapSelect_Draw and its helpers).
 * Each one expands to the full expression on purpose: the original
 * repeats the loads and the MapSelect_GetTab calls every time. */

#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/war_room_records.h"

int MapSelect_GetTab(void);

extern MapListTables data_0229c768;

/* Needed to match: through the cast the compiler keeps field offsets out of
 * the literal pool, as the original does. */
#define MENU ((MenuState*)&gMenuState)
#define TABLES data_0229c768
/* Current tab of the menu, 0-3; each tab has its own map list. */
#define TAB() MapSelect_GetTab()
#define LIST_OF_TAB(tab) (TABLES.listOfTab[tab])
#define LIST(tab) (MENU->setup.lists[LIST_OF_TAB(tab)])

/* The War Room records' CO pictures (MapSelect_DrawStatsPanel): front and
 * back CO of each place, from OBJ tile RECORD_CO_TILE and OBJ palette
 * RECORD_CO_PALETTE on, one palette each. The pictures of the previous list
 * follow them during a slide. */
#define RECORD_CO_PICTURES (WAR_ROOM_PLACE_COUNT * PLAYER_CO_COUNT)
#define RECORD_CO_TILE 0x1ca
#define RECORD_CO_PALETTE 3

/* Map list layout: COLS by ROWS maps per page. */
#define COLS                                 \
  (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM \
       ? 7                                   \
       : (MAP_STATE->twoMaps == 0 ? 9 : 8))
#define ROWS (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM ? 1 : 3)
/* Page of the selected entry; the map's index is page * ROWS * COLS plus its
 * position on the page. */
#define PAGE() (TABLES.unk_0d0[TAB()][LIST(TAB()).unk_64])
#define SELECTED_MAP_ID()                                                 \
  (LIST(TAB()).mapIds[TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()] + \
                      PAGE() * (ROWS * COLS)])

#endif
