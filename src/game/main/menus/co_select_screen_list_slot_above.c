#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/math.h"

int Campaign_IsHardMode(void);
int CoSelectScreen_GetListCo(u8 slot); /* CO in a CO list slot; 0: empty */
BOOL CoSelect_IsCoAvailable(u8 co);    /* co is in the available mask */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* A campaign map on normal mode lists only the map's own COs, in 5 rows;
 * otherwise the list has 7 rows. Needed to match: the column count tests
 * the mode though both sides are 4. */
#define MAP_CO_LIST \
  (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode())
#define LIST_COLUMNS (MAP_CO_LIST ? 4 : 4)
#define LIST_ROWS (MAP_CO_LIST ? 5 : 7)

/* Up in the CO list: the next slot above that is empty or holds an
 * available CO, wrapping from the top row to the bottom. */
int CoSelectScreen_ListSlotAbove(int slot) {
  int rows = LIST_ROWS;
  int column = HwMod32(slot, LIST_COLUMNS);

  do {
    if (slot == column) {
      slot += (rows - 1) * LIST_COLUMNS;
    } else {
      slot -= LIST_COLUMNS;
    }
  } while (CoSelectScreen_GetListCo(slot) != 0 &&
           CoSelect_IsCoAvailable(CoSelectScreen_GetListCo(slot)) == 0);
  return slot;
}
