#include "types.h"
#include "game/tab_list.h"

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);

/* Draws one medal quad at tile column x + 21, 23 and 25 of row y for each
 * threshold the entry value reaches. */
void TabList_DrawMedals(int x, int y, int row, int col) {
  u32 value = data_0215d6c4[row][col].getValue(row, col);

  if (value >= data_0215d6c4[row][col].medalThreshold[0]) {
    Bg_PutTileQuad(x + 21, y, 0x3e4, 1);
  }
  if (value >= data_0215d6c4[row][col].medalThreshold[1]) {
    Bg_PutTileQuad(x + 23, y, 0x3e4, 1);
  }
  if (value >= data_0215d6c4[row][col].medalThreshold[2]) {
    Bg_PutTileQuad(x + 25, y, 0x3e4, 1);
  }
}
