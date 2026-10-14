#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/text_print.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void MenuCursor_DrawAt(int x, int y, int width, int height);
int Ov005_GetShopItemsFromPage(
    Unk_ov005_0236bbb0* work); /* itemCount - page * 6 */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* needed to match: each block keeps its own saved pointers */
#define SWITCH_TO(state)       \
  Unk_027c0280* saved0280;     \
  Unk_02080440* saved027c;     \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Draws the current page of the shop list on the main screen: up to six
 * rows, each an icon, a name and a price. With redrawRows set it clears the
 * rows and prints the names and prices again; the icons and the menu cursor
 * are drawn every call. */
void Ov005_DrawShopPage(Unk_ov005_0236bbb0* work) {
  int rows;
  int i;

  rows = Ov005_GetShopItemsFromPage(work);
  if (rows > 6) {
    rows = 6;
  }
  if (work->redrawRows) {
    for (i = 0; i < SHOP_PAGE_ROWS; i++) {
      SWITCH_TO(data_027c0288.unk_00);
      Bg_FillRect(data_027c0280->layer0Map, 4, 5 + i * 2, 22, 2, 0);
      SWITCH_BACK();
    }
    for (i = 0; i < rows; i++) {
      ShopItem* item = &work->items[work->page * 6 + i];
      s16 palette;
      SWITCH_TO(data_027c0288.unk_00);

      palette = 0;
      /* tile attribute bits 12-15: palette 9, which
             * Ov005_SetUpShopScreen makes from palette 0 */
      if (i == work->cursorRow && work->highlightCursorRow == 1) {
        palette = 0x9000;
      }
      Text_PrintId(4, 5 + i * 2, data_027c0280->layer0Map, item->textId,
                   palette, 0x200 + i * 32);
      Number_Draw(25, 5 + i * 2, data_027c0280->layer0Map, item->price, palette,
                  0);
      SWITCH_BACK();
    }
  }
  for (i = 0; i < rows; i++) {
    ShopItem* item = &work->items[work->page * 6 + i];
    SWITCH_TO(data_027c0288.unk_00);

    Icon_Draw(item->iconId, 16, 40 + i * 16, 0, 0);
    SWITCH_BACK();
  }
  for (i = 0; i < SHOP_PAGE_ROWS; i++) {
    SWITCH_TO(data_027c0288.unk_00);
    if (i == work->cursorRow && work->showCursor == 1) {
      MenuCursor_DrawAt(16, 40 + i * 16, 200, 16);
    }
    SWITCH_BACK();
  }
}
