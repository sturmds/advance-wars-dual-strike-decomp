#include "types.h"
#include "game/tab_list.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
extern Unk_027c0280* data_027c0280;

/* Draws entry (row, col) at tile (x, y) of layer 0: its text at x and its number at x + 20, with no tile quad behind. */
void TabList_DrawEntryPlain(int x, int y, int row, int col) {
  u16* map;

  Text_PrintId(x, y, data_027c0280->layer0Map, data_0215d6c4[row][col].textId,
               0, ((y - 5) >> 1) * 64 + 128);
  map = data_027c0280->layer0Map;
  Number_Draw(x + 20, y, map, data_0215d6c4[row][col].getValue(row, col), 0, 0);
}
