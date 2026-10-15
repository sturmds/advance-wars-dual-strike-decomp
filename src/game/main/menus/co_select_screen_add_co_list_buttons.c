#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"
#include "game/math.h"

extern const MenuButtonTemplate gCoListButtonTemplate;

int Campaign_IsHardMode(void);
int CoSelectScreen_ListSlotOfCo(
    int co); /* list slot of co in data_0229c73c->coList */
BOOL CoSelect_IsCoAvailable(u8 co); /* co is in the available mask */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* A campaign map on normal mode offers only the map's own COs, in 5 rows
 * of 24 pixels; otherwise the list has 7 rows of 16 pixels. Needed to
 * match: the column count tests the mode though both sides are 4, and the
 * row count turns the test into 0 or 1 first. */
#define MAP_CO_LIST \
  (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode())
#define LIST_COLUMNS (MAP_CO_LIST ? 4 : 4)
#define LIST_ROWS ((MAP_CO_LIST ? 1 : 0) ? 5 : 7)
#define BUTTON_Y(i) \
  (MAP_CO_LIST ? HwDiv32(i, 4) * 24 + 72 : HwDiv32(i, 4) * 16 + 72)
#define BUTTON_X(i) (HwMod32(i, LIST_COLUMNS) * 56 + 8)

/* Opens the CO list for the selected grid cell: selects the button of the
 * cell's CO, then adds one button per list slot (data_0229c73c->coList)
 * that is empty or holds an available CO. */
void CoSelectScreen_AddCoListButtons(void) {
  u8* column = &data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2];
  u8* co = data_0229c73c->coList;
  long i; /* needed to match: int gets other registers */

  MenuButton_Select(
      CoSelectScreen_ListSlotOfCo(column[data_0229c73c->cursorRow]));
  for (i = 0; i < LIST_ROWS * LIST_COLUMNS; i++, co++) {
    if (*co == 0 || CoSelect_IsCoAvailable(*co) == 1) {
      MenuButton_Add(BUTTON_X(i), BUTTON_Y(i), &gCoListButtonTemplate, i);
    }
  }
}
