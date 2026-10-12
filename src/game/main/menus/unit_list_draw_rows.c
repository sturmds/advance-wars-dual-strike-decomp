#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/unit_list_task.h"

void Bg_ClearRect(void* target, int x, int y, int w, int h);
void Gfx_RequestLayerCopy(int layer);
void UnitList_DrawRow(int y, int index);
void UnitList_DrawHeadings(void);

extern Unk_027c0280* data_027c0280;

/* Redraws the list area of layer 0 for a scroll position in half rows.
 * At a half row the list is shifted up by one tile row, so one more row
 * is drawn and the rows cut off at the top and bottom are blanked
 * before the column headings are drawn again. */
void UnitList_DrawRows(int scroll) {
  int i;
  int index;
  int y;

  Bg_ClearRect(data_027c0280->layer0Map, 0, 8, 32, 12);
  if (scroll & 1) {
    index = scroll >> 1;
    i = 0;
    y = -1;
    for (; i < 7; i++) {
      UnitList_DrawRow(y, index);
      index++;
      y += 2;
    }
    Bg_ClearRect(data_027c0280->layer0Map, 0, 7, 32, 1);
    Bg_ClearRect(data_027c0280->layer0Map, 0, 20, 32, 1);
    UnitList_DrawHeadings();
  } else {
    int rowY;
    int row;

    row = 0;
    rowY = 0;
    scroll >>= 1;
    for (; row < UNIT_LIST_VISIBLE_ROWS; row++) {
      UnitList_DrawRow(rowY, scroll);
      scroll++;
      rowY += 2;
    }
  }
  Gfx_RequestLayerCopy(0);
}
