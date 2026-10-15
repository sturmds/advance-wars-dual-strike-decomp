#include "types.h"
#include "game/icon.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/math.h"

#define ICON_DIGIT_0 150
#define ICON_SLASH 162

/* Draws "page/pages" with digit icons at y 32 on the data_027c0288.unk_00
 * screen, or "0/0" when the list is empty. */
void Ov005_DrawShopPageNumber(Unk_ov005_0236bbb0* work) {
  int page;
  int pageTens;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int pages;
  int pagesTens;

  if (work->itemCount == 0) {
    page = 0;
  } else {
    page = work->page + 1;
  }
  pageTens = HwDiv32(page, 10);
  page -= pageTens * 10;
  if (pageTens == 0) {
    pageTens = -1;
  }
  if (work->itemCount == 0) {
    pages = 0;
  } else {
    pages = work->lastPage + 1;
  }
  pagesTens = HwDiv32(pages, 10);
  pages -= pagesTens * 10;
  if (pagesTens == 0) {
    pagesTens = -1;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (work->lastPage + 1 < 10) {
    Icon_Draw(ICON_DIGIT_0 + page, 184, 32, 0, 0);
    Icon_Draw(ICON_SLASH, 192, 32, 0, 0);
    Icon_Draw(ICON_DIGIT_0 + pages, 200, 32, 0, 0);
  } else {
    if (pageTens != -1) {
      Icon_Draw(ICON_DIGIT_0 + pageTens, 168, 32, 0, 0);
    }
    if (page != -1) {
      Icon_Draw(ICON_DIGIT_0 + page, 176, 32, 0, 0);
    }
    Icon_Draw(ICON_SLASH, 184, 32, 0, 0);
    if (pagesTens != -1) {
      Icon_Draw(ICON_DIGIT_0 + pagesTens, 192, 32, 0, 0);
    }
    if (pages != -1) {
      Icon_Draw(ICON_DIGIT_0 + pages, 200, 32, 0, 0);
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
