#include "types.h"
#include "game/page_strip.h"

PageStrip* data_0229c744;

void MenuButton_Add(int x, int y, void* def, int id);
void* MenuButton_Select(int id);
void MenuButtons_RemoveSelectable(void);
int HwMod32(int num, int denom);
int SurvivalSelect_GetList(void);

extern u8 data_0215ac24[];
extern u8 data_0215ad14[];

/* Adds one button per entry of the current list in rows of 15: the selected
 * entry is 32 pixels wide, the others 16. Then selects the cursor entry.
 * Name and purpose are guesses. */
void PageStrip_AddButtons(void) {
  int list = SurvivalSelect_GetList();
  int x = 0;
  int y = 0;
  int i;

  MenuButtons_RemoveSelectable();
  for (i = 0; i < data_0229c744->lists[list].count; i++) {
    if (i == data_0229c744->lists[list].cursor) {
      MenuButton_Add(x, y, data_0215ac24, i);
      if (HwMod32(i + 1, 15) == 0) {
        x = 0;
        y += 32;
      } else {
        x += 32;
      }
    } else {
      MenuButton_Add(x, y, data_0215ad14, i);
      if (HwMod32(i + 1, 15) == 0) {
        x = 0;
        y += 32;
      } else {
        x += 16;
      }
    }
  }
  MenuButton_Select(data_0229c744->lists[list].cursor);
}
