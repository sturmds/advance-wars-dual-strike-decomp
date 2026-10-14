#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/event_script.h"
#include "game/task.h"

int Ov005_GetShopItemsFromPage(Unk_ov005_0236bbb0* work);

void Ov005_AllocShopWork(void);
void Ov005_MoveShopCursorOnKeys(void);
void Ov005_RequestShopLeave(void);
void Ov005_RequestShopNextPage(void);
void Ov005_RequestShopPrevPage(void);
void Ov005_SetUpShopScreen(void);
void Ov005_UpdateShopScreen(void);
extern u8 data_ov005_0236b900[];
void func_ov005_02365ec0(void);
void Ov005_RequestShopBuy(void);
extern u8 data_ov005_0236ba40[];
void Ov005_SetShopCursorRow(void);
extern u8 data_ov005_0236ba80[];
extern u8 data_ov005_0236b6c0[];
extern u8 data_ov005_0236b680[];
extern u8 data_ov005_0236b640[];
extern u8 data_ov005_0236b5c0[];
extern u8 data_ov005_0236b740[];
extern u8 data_ov005_0236b7c0[];
extern u8 data_ov005_0236b940[];
extern u8 data_ov005_0236bac0[];
extern u8 data_ov005_0236b580[];
extern u8 data_ov005_0236b600[];
extern u8 data_ov005_0236b800[];
extern u8 data_ov005_0236bb00[];
extern u8 data_ov005_0236b700[];
extern u8 data_ov005_0236bb40[];

MenuButtonTemplate data_ov005_0236b42c = {
#include "data/ov005/data_ov005_0236b42c.inc.c"
};
MenuButtonTemplate data_ov005_0236b404 = {
#include "data/ov005/data_ov005_0236b404.inc.c"
};
static MenuButtonTemplate data_ov005_0236b3dc = {
#include "data/ov005/data_ov005_0236b3dc.inc.c"
};
MenuButtonTemplate data_ov005_0236b3b4 = {
#include "data/ov005/data_ov005_0236b3b4.inc.c"
};
TaskScriptCmd data_ov005_0236b38c[5] = {
#include "data/ov005/data_ov005_0236b38c.inc.c"
};
const EventScriptCommand* data_ov005_0236b454[17] = {
    (EventScriptCommand*)&data_ov005_0236b900,
    (EventScriptCommand*)&data_ov005_0236ba40,
    (EventScriptCommand*)&data_ov005_0236ba80,
    (EventScriptCommand*)&data_ov005_0236b6c0,
    (EventScriptCommand*)&data_ov005_0236b680,
    (EventScriptCommand*)&data_ov005_0236b640,
    (EventScriptCommand*)&data_ov005_0236b5c0,
    (EventScriptCommand*)&data_ov005_0236b740,
    (EventScriptCommand*)&data_ov005_0236b7c0,
    (EventScriptCommand*)&data_ov005_0236b940,
    (EventScriptCommand*)&data_ov005_0236bac0,
    (EventScriptCommand*)&data_ov005_0236b580,
    (EventScriptCommand*)&data_ov005_0236b600,
    (EventScriptCommand*)&data_ov005_0236b800,
    (EventScriptCommand*)&data_ov005_0236bb00,
    (EventScriptCommand*)&data_ov005_0236b700,
    (EventScriptCommand*)&data_ov005_0236bb40};

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
