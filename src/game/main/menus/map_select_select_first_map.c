#include "types.h"
#include "game/map_select.h"

extern MenuState gMenuState;

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void MapSelect_DrawMapNames(int list, int page);
void MapSelect_AddMapButtons(void);

/* After the lists are rebuilt for the second pick of a two-map battle:
 * finds the first map (firstMapId) in the new lists, makes its list the current
 * tab's list, turns to its page and selects it, then redraws the map names
 * and buttons. The map must be in one of the lists. */
void MapSelect_SelectFirstMap(void) {
  int list;
  int index;

  list = 0;
  index = 0;
  for (;;) {
    if (index >= MENU->setup.lists[list].mapCount) {
      index = 0;
      list++;
    }
    if (MENU->setup.lists[list].mapIds[index] == gMenuState.setup.firstMapId) {
      break;
    }
    index++;
  }
  LIST_OF_TAB(TAB()) = list;
  PAGE() = HwDiv32(index, ROWS * COLS);
  TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()] =
      HwMod32(index, ROWS * COLS);
  MapSelect_DrawMapNames(LIST_OF_TAB(TAB()), PAGE());
  MapSelect_AddMapButtons();
}
