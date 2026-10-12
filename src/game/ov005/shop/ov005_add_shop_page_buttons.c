#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"

int Ov005_GetShopItemsFromPage(Unk_ov005_0236bbb0* work);

extern const MenuButtonTemplate data_ov005_0236b3dc;

/* Adds one menu button (ids 0x80 up) on the main screen for each item on
 * the current page, then selects work->cursorRow, clamped to the page. */
void Ov005_AddShopPageButtons(Unk_ov005_0236bbb0* work) {
  ShopButton* slot;
  int i;
  int count;

  count = Ov005_GetShopItemsFromPage(work);
  if (count > 6) {
    count = 6;
  }
  work->buttonCount = count;
  for (i = 0; i < count; i++) {
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    slot = &work->buttons[i];
    slot->shown = 1;
    slot->buttonId = i + 0x80;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    MenuButton_Add(slot->x, slot->y, &data_ov005_0236b3dc, slot->buttonId);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (work->itemCount == 0) {
    return;
  }
  if (work->cursorRow >= count) {
    work->cursorRow = count - 1;
  }
  MenuButton_Select(work->buttons[work->cursorRow].buttonId);
}
