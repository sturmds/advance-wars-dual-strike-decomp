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
 * the mode though both sides are 4, and the row count turns the test into
 * 0 or 1 first. */
#define MAP_CO_LIST \
  (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode())
#define LIST_COLUMNS (MAP_CO_LIST ? 4 : 4)
#define LIST_ROWS ((MAP_CO_LIST ? 1 : 0) ? 5 : 7)

/* Left in the CO list: in the previous column (wrapping to the last), the
 * first slot that is empty or holds an available CO, looking from the
 * listAnchor row up to the top, then from it down to the bottom. A column
 * with no such slot is skipped. */
int CoSelectScreen_ListSlotLeft(int slot) {
  int column = HwMod32(slot, LIST_COLUMNS);
  int anchorRow = HwDiv32(data_0229c73c->listAnchor, LIST_COLUMNS);
  int row;
  int co;

  for (;;) {
    if (column == 0) {
      column = LIST_COLUMNS - 1;
    } else {
      column--;
    }
    for (row = anchorRow; row >= 0; row--) {
      co = CoSelectScreen_GetListCo(column + row * LIST_COLUMNS);
      if (co == 0 || CoSelect_IsCoAvailable(co) == 1) {
        return column + row * LIST_COLUMNS;
      }
    }
    for (row = anchorRow; row < LIST_ROWS; row++) {
      co = CoSelectScreen_GetListCo(column + row * LIST_COLUMNS);
      if (co == 0 || CoSelect_IsCoAvailable(co) == 1) {
        return column + row * LIST_COLUMNS;
      }
    }
  }
}
