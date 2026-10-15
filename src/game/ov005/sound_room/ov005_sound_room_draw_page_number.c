#include "types.h"
#include "game/icon.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/math.h"

#define PAGE_COUNT \
  ((OV005_CO_CELL_COUNT + OV005_CELLS_PER_PAGE - 1) / OV005_CELLS_PER_PAGE)

#define ICON_DIGIT_0 150
#define ICON_SLASH 162

/* Draws "page/pages" with digit icons at y 48 on the data_027c0288.unk_00
 * screen: one digit each while there are fewer than 10 pages, otherwise
 * two-digit fields with the leading zero left out. */
void Ov005SoundRoom_DrawPageNumber(Unk_ov005_0236bbac* work) {
  int page;
  int pageTens;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int pages;
  int pagesTens;

  page = work->page + 1;
  pageTens = HwDiv32(page, 10);
  page -= pageTens * 10;
  if (pageTens == 0) {
    pageTens = -1;
  }
  pages = PAGE_COUNT;
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
    Icon_Draw(ICON_DIGIT_0 + page, 184, 48, 0, 0);
    Icon_Draw(ICON_SLASH, 192, 48, 0, 0);
    Icon_Draw(ICON_DIGIT_0 + pages, 200, 48, 0, 0);
  } else {
    if (pageTens != -1) {
      Icon_Draw(ICON_DIGIT_0 + pageTens, 168, 48, 0, 0);
    }
    if (page != -1) {
      Icon_Draw(ICON_DIGIT_0 + page, 176, 48, 0, 0);
    }
    Icon_Draw(ICON_SLASH, 184, 48, 0, 0);
    if (pagesTens != -1) {
      Icon_Draw(ICON_DIGIT_0 + pagesTens, 192, 48, 0, 0);
    }
    if (pages != -1) {
      Icon_Draw(ICON_DIGIT_0 + pages, 200, 48, 0, 0);
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
